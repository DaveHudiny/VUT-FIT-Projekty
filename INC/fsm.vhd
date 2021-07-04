-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (inX, in2, in29, in292, in2922, in29229, in292290, in2922902, in29229020, in292290209, 
	in2922902092, in26, in265, in2653, in26536, in265362, in2653627, in26536276, in265362761, in2653627616, inNX, PRINT_NOPE, PRINT_YEEP, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= inX;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when inX =>
      next_state <= inX;
		if (KEY(2)='1') then
			next_state <= in2;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when inNX =>
      next_state <= inNX;
      if(KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in2 =>
      next_state <= in2;
		if (KEY(9)='1') then
			next_state <= in29;
		elsif(KEY(6)='1') then
			next_state <= in26;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in29 =>
      next_state <= in29;
		if (KEY(2)='1') then
			next_state <= in292;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in292 =>
      next_state <= in292;
		if (KEY(2)='1') then
			next_state <= in2922;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in2922 =>
      next_state <= in2922;
		if (KEY(9)='1') then
			next_state <= in29229;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in29229 =>
      next_state <= in29229;
		if (KEY(0)='1') then
			next_state <= in292290;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in292290 =>
      next_state <= in292290;
		if (KEY(2)='1') then
			next_state <= in2922902;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in2922902 =>
      next_state <= in2922902;
		if (KEY(0)='1') then
			next_state <= in29229020;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in29229020 =>
      next_state <= in29229020;
		if (KEY(9)='1') then
			next_state <= in292290209;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in292290209 =>
      next_state <= in292290209;
		if (KEY(2)='1') then
			next_state <= in2922902092;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in2922902092 =>
      next_state <= in2922902092;
      if (KEY(15) = '1') then
         next_state <= PRINT_YEEP; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in26 =>
      next_state <= in26;
		if (KEY(5)='1') then
			next_state <= in265;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in265 =>
      next_state <= in265;
		if (KEY(3)='1') then
			next_state <= in2653;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in2653 =>
      next_state <= in2653;
		if (KEY(6)='1') then
			next_state <= in26536;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in26536 =>
      next_state <= in26536;
		if (KEY(2)='1') then
			next_state <= in265362;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in265362 =>
      next_state <= in265362;
		if (KEY(7)='1') then
			next_state <= in2653627;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in2653627 =>
      next_state <= in2653627;
		if (KEY(6)='1') then
			next_state <= in26536276;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in26536276 =>
      next_state <= in26536276;
		if (KEY(1)='1') then
			next_state <= in265362761;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in265362761 =>
      next_state <= in265362761;
		if (KEY(6)='1') then
			next_state <= in2653627616;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_NOPE; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when in2653627616 =>
      next_state <= in2653627616;
		if (KEY(15) = '1') then
         next_state <= PRINT_YEEP; 
		elsif(KEY(14 downto 0) /= "000000000000000") then
			next_state <= inNX;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_NOPE =>
      next_state <= PRINT_NOPE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when PRINT_YEEP =>
      next_state <= PRINT_YEEP;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= inX; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= inX;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when inX | inNX | in2 | in29 | in292 | in2922 | in29229 | in292290 | in2922902 | in29229020 | in292290209 | 
	in2922902092 | in26 | in265 | in2653 | in26536 | in265362 | in2653627 | in26536276 | in265362761 | in2653627616 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_NOPE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM		<= '0';
   -- - - - - - - - - - - - - - - - - - - - - - -
	when PRINT_YEEP =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM		<= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   end case;
end process output_logic;

end architecture behavioral;

