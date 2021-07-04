-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2020 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WE    : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti 
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is
	-- Program counter
	signal PC_reg : std_logic_vector(11 downto 0);
	signal PC_ld  : std_logic;
	signal PC_inc : std_logic;
	signal PC_dec : std_logic;
	
	-- RAS
	signal RAS_adr : std_logic_vector(11 downto 0);
	signal RAS_push: std_logic;
	signal RAS_pop : std_logic;

	-- CNT
	signal CNT_dep : std_logic_vector(11 downto 0);
	signal CNT_inc : std_logic;
	signal CNT_dec : std_logic;

	-- PTR
	signal PTR_adr : std_logic_vector(9 downto 0);
	signal PTR_inc : std_logic;
	signal PTR_dec : std_logic;
	

	-- ALU
	signal ALU_sel : std_logic_vector(1 downto 0);
	signal ALU_res : std_logic_vector(7 downto 0);

	-- FSM
	type FSM is(
	-- basic
		s_start,
		s_fetch,
		s_decode,
	-- pointer
		s_ptr_inc,
		s_ptr_dec,
	-- value
		s_val_inc,
		s_val_ALU_inc,
		s_val_end_inc,

		s_val_dec,
		s_val_ALU_dec,
		s_val_end_dec,
	-- while
		s_loop_start,
		s_first_cond,
		s_do_nothing,


		s_loop_end,
		s_last_cond,
	-- read/write
		s_write,
		s_write_or_busy,

		s_get,
		s_get_end,

		s_null
	);

	signal state : FSM := s_start;
	signal nxt_state : FSM := s_fetch;
 -- zde dopiste potrebne deklarace signalu

begin

 -- zde dopiste vlastni VHDL kod

	-- PC

	PC: process(CLK, RESET, PC_inc, PC_dec, PC_ld) is
	begin
		if RESET = '1' then
			PC_reg <= (others => '0');
		elsif CLK'EVENT and CLK = '1' then
			if PC_inc = '1' then
				PC_reg <= PC_reg + 1;
			elsif PC_dec = '1' then
				PC_reg <= PC_reg - 1;
			elsif PC_ld = '1' then
				PC_reg <= RAS_adr;
			end if;
		end if;
	end process;
	CODE_ADDR <= PC_reg;

	-- RAS

	RAS: process(CLK, RESET, RAS_push, RAS_pop)
	variable stack : std_logic_vector (183 downto 0) := (others => '0');
	begin
		if RESET = '1' then
			stack(183 downto 0) := (others => '0');
		elsif CLK'EVENT and CLK = '1' then
			RAS_adr <= stack(11 downto 0);
			if RAS_push = '1' then
				stack := stack(171 downto 0) & PC_reg; 
			elsif RAS_pop = '1' then
				stack(171 downto 0) := stack(183 downto 12);
				stack(183 downto 172) := (others => '0');
			end if;
		end if;
		
	end process;

	-- CNT

	CNT: process(CLK, RESET, CNT_inc, CNT_dec)
	begin
		if RESET = '1' then
			CNT_dep <= (others => '0');
		elsif CLK'EVENT and CLK = '1' then
			if CNT_inc = '1' then
				CNT_dep <= CNT_dep + 1;
			elsif CNT_dec = '1' then
				CNT_dep <= CNT_dep - 1;
			end if;
		end if;
	end process;

	-- PTR

	PTR: process(CLK, RESET, PTR_inc, PTR_dec) is
	begin
		if RESET = '1' then
			PTR_adr <= (others => '0');
		elsif CLK'EVENT and CLK = '1' then
			if    PTR_inc = '1' then
				PTR_adr <= PTR_adr + 1;
			elsif PTR_dec = '1' then
				PTR_adr <= PTR_adr - 1;
			end if;
		end if;
	end process;

	DATA_ADDR <= PTR_adr;

	-- ALU

	ALU: process(CLK, RESET, ALU_sel)
	begin
		if RESET = '1' then
			ALU_res <= (others => '0');
		elsif CLK'EVENT and CLK = '1' then
			if ALU_sel = "00" then
				ALU_res <= IN_DATA;
			elsif ALU_sel = "01" then
				ALU_res <= DATA_RDATA + 1;
			elsif ALU_sel = "10" then
				ALU_res <= DATA_RDATA - 1;
			else
				ALU_res <= (others => '0');
			end if;
		end if;

	end process;
	DATA_WDATA <= ALU_res;

	-- FSM

	LOGIC: process (CLK, RESET, EN) is
	begin
		if RESET = '1' then
			state <= s_start;
		elsif CLK'EVENT and CLK = '1' then
			if EN = '1' then
				state <= nxt_state;
			end if;
		end if;
	end process;

	FSM_automat: process (CLK, OUT_BUSY, IN_VLD, CODE_DATA, DATA_RDATA, CNT_dep, state) is
	begin
		-- INIT	
		PC_inc <= '0';
		PC_dec <= '0';
		PC_ld <= '0';
		
		RAS_push <= '0';
		RAS_pop <=  '0';

		CNT_INC <=  '0';
		CNT_dec <=  '0';

		PTR_inc <=  '0';
		PTR_dec <=  '0';

		ALU_sel <= "00";

		CODE_EN <=  '0';
		DATA_EN <=  '0';
		DATA_WE <=  '0';
		OUT_WE <=  '0';
		IN_REQ <=  '0';


		case state is
			when s_start =>
				nxt_state <= s_fetch;
			when s_fetch =>
				CODE_EN <= '1';
				nxt_state <= s_decode;
			when s_decode =>

				case CODE_DATA is
					when x"3E" =>
						nxt_state <= s_ptr_inc;
					when x"3C" =>
						nxt_state <= s_ptr_dec;
					when x"2B" =>
						nxt_state <= s_val_inc;
					when x"2D" =>
						nxt_state <= s_val_dec;
					when x"5B" =>
						nxt_state <= s_loop_start;
					when x"5D" =>
						nxt_state <= s_loop_end;
					when x"2E" =>
						nxt_state <= s_write;
					when x"2C" =>
						nxt_state <= s_get;
					when x"00" =>
						nxt_state <= s_null;
					when others =>
						PC_inc <= '1';
						nxt_state <= s_fetch;
				end case;

			when s_ptr_inc =>
				PTR_inc <= '1';
				PC_inc <= '1';
				nxt_state <= s_fetch;
			when s_ptr_dec =>
				PTR_dec <= '1';
				PC_inc <= '1';
				nxt_state <= s_fetch;

			when s_val_inc =>
				DATA_EN <= '1';
				DATA_WE <= '0';
				nxt_state <= s_val_ALU_inc;
			when s_val_ALU_inc =>
				ALU_sel <= "01";
				nxt_state <= s_val_end_inc;
			when s_val_end_inc =>
				DATA_EN <= '1';
				DATA_WE <= '1';
				PC_inc <= '1';
				nxt_state <= s_fetch;

			when s_val_dec =>
				DATA_EN <= '1';
				DATA_WE <= '0';
				nxt_state <= s_val_ALU_dec;
			when s_val_ALU_dec =>
				ALU_sel <= "10";
				nxt_state <= s_val_end_dec;
			when s_val_end_dec =>
				DATA_EN <= '1';
				DATA_WE <= '1';
				PC_inc <= '1';
				nxt_state <= s_fetch;
			
			when s_write =>
				DATA_EN <= '1';
				DATA_WE <= '0';
				nxt_state <= s_write_or_busy;
			when s_write_or_busy =>
				if OUT_BUSY = '1' then
					DATA_EN <= '1';
					DATA_WE <= '0';
					nxt_state <= s_write_or_busy;
				else
					OUT_WE <= '1';
					PC_inc <= '1';
					nxt_state <= s_fetch;
				end if;
			
			when s_get =>
				IN_REQ <= '1';
				ALU_sel <= "00";
				if IN_VLD = '0' then
					nxt_state <= s_get;
				else
					nxt_state <= s_get_end;
				end if;
			when s_get_end =>
				DATA_EN <= '1';
				DATA_WE <= '1';
				PC_inc <= '1';
				nxt_state <= s_fetch;

			when s_loop_start =>
				PC_inc <= '1';
				DATA_EN <= '1';
				DATA_WE <= '0';
				nxt_state <= s_first_cond;

			when s_first_cond =>
				if DATA_RDATA = "00000000" then
					CNT_inc <= '1';
					CODE_EN <= '1';
					nxt_state <= s_do_nothing;
				else
					RAS_push <= '1';
					nxt_state <= s_fetch;
				end if;

			when s_do_nothing =>
				if CNT_dep = "00000000" then
					PC_dec <= '1';
					nxt_state <= s_fetch;
				else
					PC_inc <= '1';
					CODE_EN <= '1';
					nxt_state <= s_do_nothing;
					if CODE_DATA = x"5B" then
						CNT_inc <= '1';
					elsif CODE_DATA = x"5D" then
						CNT_dec <= '1';
					end if;
				end if;

			when s_loop_end =>
				DATA_EN <= '1';
				DATA_WE <= '0';
				nxt_state <= s_last_cond;

			when s_last_cond =>
				if DATA_RDATA = "00000000" then
					PC_inc <= '1';
					RAS_pop <= '1';
					nxt_state <= s_fetch;
				else 
					PC_ld <= '1';
					nxt_state <= s_fetch;
				end if;

			when s_null =>
				nxt_state <= s_null;
				null;
			when others =>
				nxt_state <= state;

		end case;



					



	end process;
	OUT_DATA <= DATA_RDATA;


 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --   - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --   - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly.
 
end behavioral;
 
