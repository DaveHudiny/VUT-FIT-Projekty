isa_protocol = Proto("ISA",  "ISA project dissector")

source = ProtoField.string("isa_protocol.source", "Message source", base.ASCII) -- Some useful declarations
response_line = ProtoField.string("isa_protocol.response_line", "Result", base.ASCII)
command_line = ProtoField.string("isa_protocol.command_line", "Command", base.ASCII)

user_line = ProtoField.string("isa_protocol.user_line", "User", base.ASCII)
token_line = ProtoField.string("isa_protocol.token_line", "Token", base.ASCII)
password_line = ProtoField.string("isa_protocol.password_line", "Password (hashed)", base.ASCII)
id_line = ProtoField.string("isa_protocol.id_line", "ID", base.int32)
message_counter = ProtoField.string("isa_protocol.message_counter", "Message count", base.int32)
subject_line = ProtoField.string("isa_protocol.subject_line", "Subject", base.ASCII)
message_line = ProtoField.string("isa_protocol.message_line", "Text", base.ASCII)

isa_protocol.fields = {source, response_line, command_line, user_line, message_line, -- Very important to be printable
	token_line, password_line, id_line, message_counter, subject_line}

function isa_protocol.dissector(buffer, pinfo, tree) -- "main"
	length = buffer:len()
	if length == 0 then return end

	pinfo.cols.protocol = isa_protocol.name -- some formalities

	local subtree = tree:add(isa_protocol, buffer(), "ISA project protocol")

	local message_three = buffer(1, 3):string() -- err or ok or something else
	local source_device = get_source(message_three) -- is this message from client or server?
	subtree:add_le(source, source_device)
	local command = "unknown"

	if source_device == "Client message" then command = get_command(buffer():string()) -- command will be fetch or login etc
		subtree:add_le(command_line, command)
	elseif source_device == "Server response" then command = message_three -- command will be err or "ok "
		subtree:add_le(response_line, command) 
	end
	
	-- decision tree for each command. Each command has it's own functions. Simply gets strings which are important for each command
	if 	  command == "fetch"    then
		local token, id = getFetch(buffer():string())
		subtree:add_le(token_line, token)
		subtree:add_le(id_line, id)
	elseif  command == "login"    then
		local user, password = getLogin(buffer():string())
		subtree:add_le(user_line, user)
		subtree:add_le(password_line, password)
	elseif  command == "register" then
		local user, password = getLogin(buffer():string())
		subtree:add_le(user_line, user)
		subtree:add_le(password_line, password)
	elseif  command == "logout"   then
		local token =  getErrMes(buffer():string())
		subtree:add_le(token_line, token)
	elseif  command == "send"     then
		local token, user, subject, mess = getSend(buffer():string())
		subtree:add_le(user_line, user)
		subtree:add_le(subject_line, subject)
		subtree:add_le(message_line, mess)
		subtree:add_le(token_line, token)
	elseif  command == "list"     then
		local token =  getErrMes(buffer():string())
		subtree:add_le(token_line, token)
	elseif  command == "err" 	then
		local errMes = getErrMes(buffer():string())
		subtree:add_le(message_line, errMes)  
	elseif  command == "ok "	then -- from here it gets more interesting - we need do deduce which command was server responding to
		local which = getOk(buffer():string())
		if which == "send or login or register or logout" then -- We find that the message doesnt contain any intern left parenthesis. It implies that our options are smaller then before.
			local number, mess1, mess2 = getSendLoginRegisterLogout(buffer():string()) -- We read one or two strings for messages (for example ok "user logged" "hash" has two messages, other commands have only one)
			print(number)
			if number == 2 then
				subtree:add_le(command_line, "login")
				subtree:add_le(message_line, mess1)
				subtree:add_le(token_line, mess2)
			elseif number == 1 then
				if mess1 == "user logged out" then
					subtree:add_le(command_line, "logout")
				elseif mess1:sub(1,4) == "regi" then
					subtree:add_le(command_line, "register")
				elseif mess1:sub(1,4) == "mess" then
					subtree:add_le(command_line, "send")
				else
					subtree:add_le(command_line, "unknown")
				end
				subtree:add_le(message_line, mess1)
			else 
				subtree:add_le(command_line, "unknown command")
			end
		elseif which == "list or fetch" then -- we find that there are some internal left parenthesis so our only options are list or fetch 
			local which = getListOrFetch(buffer():string())
			subtree:add_le(command_line, which)
			if which == "list" then
				subtree:add_le(message_counter, getNumberOfMessages(buffer():string()))
			elseif which == "fetch" then 
				local from, subject, body = getResponseFetch(buffer():string())
				subtree:add_le(user_line, from)
				subtree:add_le(subject_line, subject)
				subtree:add_le(message_line, body)
			end
		end
	end
  
end

-- function returns count of messages, which are contained in list response
function getNumberOfMessages(message)
	local i = 0
	local special = false
	local inside = false
	for c in message:gmatch"." do
		if c == '\\' or special then
			special = not special
		elseif c == '\"' and not special then
			inside = not inside	
		elseif not inside and c == '(' then
			i = i + 1
		end
	end
	return i - 2
end

-- function returns message from fetch command. It contains three strings -- one for sender, one for subject and one for body
function getResponseFetch(message)
	to = ""
	subject = ""
	body = ""
	i = 0
	for c in message:gmatch"." do
		if (c == '\\' and inside) or special then
			special = not special
			if i == 0 then
				to = to .. c
			elseif i == 1 then
				subject = subject .. c
			elseif i == 2 then
				body = body .. c
			end
		elseif c == '\"' and not special then
			if inside then
				i = i + 1 
			end			
			inside = not inside	
		elseif inside then
			if i == 0 then
				to = to .. c
			elseif i == 1 then
				subject = subject .. c
			elseif i == 2 then
				body = body .. c
			end	
		end
	end
	return to, subject, body

end

-- function decides whether message is response for fetch or for list command
function getListOrFetch(message)
	for c in message:gmatch"." do
		if c == '\"' then
			return "fetch"
		elseif c >= '0' and c <= '9' then
			return "list"
		end
	end 
end

-- function reads number of strings and content of them. It helps to decide which command it is in main function
function getSendLoginRegisterLogout(message)
	local i = 0
	message1 = ""
	message2 = ""
	i = 0
	for c in message:gmatch"." do
		if (c == '\\' and inside) or special then
			special = not special
			if i == 0 then
				message1 = message1 .. c
			elseif i == 1 then
				message2 = message2 .. c
			end
		elseif c == '\"' and not special then
			if inside then
				i = i + 1 
			end			
			inside = not inside	
		elseif inside then
			if i == 0 then
				message1 = message1 .. c
			elseif i == 1 then
				message2 = message2 .. c
			end	
		end
	end
	return i, message1, message2
end

-- function decides if there is any internal left parenthesis, which implies type of command (list or fetch or other)
function getOk(message)
	local withoutFirst = message:sub(2)
	for c in withoutFirst:gmatch"." do
		if c == '\"' then
			return "send or login or register or logout"
		elseif c == '(' then
			return "list or fetch"
		end
	end 
	
end

-- function returns strings which are send with function send. Token, recipient, subject and body.
function getSend(message)
	token = ""
	to = ""
	subject = ""
	body = ""
	i = 0
	for c in message:gmatch"." do -- longest FSM
		if (c == '\\' and inside) or special then
			special = not special
			if i == 0 then
				token = token .. c
			elseif i == 1 then
				to = to .. c
			elseif i == 2 then
				subject = subject .. c
			elseif i == 3 then
				body = body .. c
			end
		elseif c == '\"' and not special then
			if inside then
				i = i + 1 
			end			
			inside = not inside	
		elseif inside then
			if i == 0 then
				token = token .. c
			elseif i == 1 then
				to = to .. c
			elseif i == 2 then
				subject = subject .. c
			elseif i == 3 then
				body = body .. c
			end	
		end
	end
	return token, to, subject, body

end

-- Function reads login type messages - it returns two strings for user and password (it can be used also for other functions if used wisely)
function getLogin(message)
	user = ""
	password = ""
	i = 0
	for c in message:gmatch"." do
		if (c == '\\' and inside) or specia then
			special = not special
			if i == 0 then
				user = user .. c
			elseif i == 1 then
				password = password .. c
			end
		elseif c == '\"' and not special then
			if inside then
				i = i + 1 
			end			
			inside = not inside	
		elseif inside then
			if i == 0 then
				user = user .. c
			elseif i == 1 then
				password = password .. c
			end	
		end
	end
	return user, password
end

-- function returns hash and id from fetch message to server.
function getFetch(message)
	token = ""
	idString = ""
	for c in message:gmatch"." do
		if (c == '\\' and inside) or special then
			special = not special
			token = token .. c
		elseif c == '\"' and not special then
			inside = not inside		
		elseif inside then
			token = token .. c
		elseif not inside and c >= '0' and c <= '9' then
			idString = idString .. c	
		end
	end
	
	return token, tonumber(idString)
end

-- function reads error message (for exapmle "err \"incorrect token\"" returns "incorrect token"
function getErrMes(message)
	local inside = false
	local result = ""
	local special = false
	for c in message:gmatch"." do
		if (c == '\\' and inside) or special then
			special = not special
			result = result .. c
		elseif c == '\"' and not special then
			inside = not inside		
		elseif inside then
			result = result .. c
		end
	end
	return result
end

-- function decides if the message is from server or from client
function get_source(message)
  local opcode_name = "Client message"
      if message == "ok " then opcode_name = "Server response"
  elseif message == "err" then opcode_name = "Server response" end

  return opcode_name
end


-- function decides which function was called from client
function get_command(message)
  	local command = "unknown"
	if     string.sub(message, 2, 6) == "login"    then command = "login"
	elseif string.sub(message, 2, 6) == "fetch"    then command = "fetch" 
	elseif string.sub(message, 2, 5) == "list"     then command = "list"
	elseif string.sub(message, 2, 5) == "send"     then command = "send"
	elseif string.sub(message, 2, 7) == "logout"   then command = "logout"
	elseif string.sub(message, 2, 9) == "register" then command = "register"
	end
  	return command

end

-- some useful stuff
local tcp_port = DissectorTable.get("tcp.port")
tcp_port:add(32323, isa_protocol)