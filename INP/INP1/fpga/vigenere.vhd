library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- login: xhudak03, jméno: David Hudák
-- rozhrani Vigenerovy sifry
entity vigenere is
   port(
         CLK : in std_logic;
         RST : in std_logic;
         DATA : in std_logic_vector(7 downto 0);
         KEY : in std_logic_vector(7 downto 0);

         CODE : out std_logic_vector(7 downto 0)
    );
end vigenere;

-- V souboru fpga/sim/tb.vhd naleznete testbench, do ktereho si doplnte
-- znaky vaseho loginu (velkymi pismeny) a znaky klice dle vaseho prijmeni.

architecture behavioral of vigenere is

    -- Sem doplnte definice vnitrnich signalu, prip. typu, pro vase reseni,
    -- jejich nazvy doplnte tez pod nadpis Vigenere Inner Signals v souboru
    -- fpga/sim/isim.tcl. Nezasahujte do souboru, ktere nejsou explicitne
    -- v zadani urceny k modifikaci.
	signal shift: std_logic_vector(7 downto 0);
	signal plusCorrection: std_logic_vector(7 downto 0);
	signal minusCorrection: std_logic_vector(7 downto 0);

	type tState is (plus, minus);
	signal actualState: tState := plus;
	signal nextState: tState := minus;

	signal mealyOutput: std_logic_vector(1 downto 0);

	-- "00" plus
	-- "01" minus
	-- "10" #
	-- "11" not defined



begin
	shiftProcess: process(DATA, KEY) is
	begin
		shift <= KEY - 64;
	end process;

	plusProcess: process (shift, DATA) is
		variable pom: std_logic_vector(7 downto 0);
	begin
		pom := DATA;
		pom := DATA+shift;
		if(pom>90) then
			pom := 64+pom-90;
		end if;

		plusCorrection <= pom;
	end process;

	minusProcess: process (shift, DATA) is
		variable pom: std_logic_vector(7 downto 0);
	begin
		pom := DATA;
		pom := DATA-shift;
		if(pom<65) then
			pom := 90+pom-64;
		end if;

		minusCorrection <= pom;
	end process;

	stateFSM: process (CLK, RST) is
	begin
		if RST = '1' then
			actualState <= plus;
		elsif CLK'EVENT and CLK = '1' then
			actualState <= nextState;
		end if;
	end process;

	fsm_mealy: process (actualState, DATA, RST) is
	begin
		case actualState is
			when plus =>
				nextState <= minus;
				mealyOutput <= "00";
			when minus =>
				nextState <= plus;
				mealyOutput <= "01";
		end case;

		if DATA > 90 or DATA < 65 or RST = '1' then
			mealyOutput <= "10";
		end if;

	end process;

	with mealyOutput select
   		CODE <= plusCorrection when "00",
        	        minusCorrection when "01",
        	        "00100011" when "10",
        	        "11111111" when others;
	



    -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.


end behavioral;
