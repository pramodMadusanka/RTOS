library ieee;
use ieee.std_logic_1164.all;

entity display is
	port(SW: in std_logic_vector(17 downto 0);
		  HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7: out std_logic_vector(0 to 6));
end display;

architecture behavioral of display is
	component sevenSegmentDecoder_Mux
		port(CH0,CH1,CH2,CH3,CH4,CH5,CH6,CH7: in std_logic_vector(2 downto 0);
			  SEL: in std_logic_vector(2 downto 0);
			  HEX: out std_logic_vector(0 to 6));
	end component;
	
	signal CH0,CH1,CH2,CH3,CH4,BLANK,SEL: std_logic_vector(2 downto 0);
	
begin	
	-- |14|13|12|11|10|9|8|7|6|5|4|3|2|1|0|
	-- '--------"-------"-----"-----"-----'
	--    000      001    010   010   011
	--     H        E      L     L     O

	SEL<=SW(17 downto 15);
	CH0<=SW(14 downto 12); --H
	CH1<=SW(11 downto 9);  --E
	CH2<=SW(8 downto 6);	  --L
	CH3<=SW(5 downto 3);   --L
	CH4<=SW(2 downto 0);   --O
	BLANK<="111";
	
	
	seg7: sevenSegmentDecoder_Mux
	port map(CH0=>BLANK, CH1=>BLANK, CH2=>BLANK, CH3=>CH0, CH4=>CH1, CH5=>CH2, CH6=>CH3, CH7=>CH4, SEL=>SEL, HEX=>HEX7);
	
	seg6: sevenSegmentDecoder_Mux
	port map(CH0=>BLANK, CH1=>BLANK, CH2=>CH0, CH3=>CH1, CH4=>CH2, CH5=>CH3, CH6=>CH4, CH7=>BLANK, SEL=>SEL, HEX=>HEX6);
	
	seg5: sevenSegmentDecoder_Mux
	port map(CH0=>BLANK, CH1=>CH0, CH2=>CH1, CH3=>CH2, CH4=>CH3, CH5=>CH4, CH6=>BLANK, CH7=>BLANK, SEL=>SEL, HEX=>HEX5);
	
	seg4: sevenSegmentDecoder_Mux
	port map(CH0=>CH0, CH1=>CH1, CH2=>CH2, CH3=>CH3, CH4=>CH4, CH5=>BLANK, CH6=>BLANK, CH7=>BLANK, SEL=>SEL, HEX=>HEX4);
	
	seg3: sevenSegmentDecoder_Mux
	port map(CH0=>CH1, CH1=>CH2, CH2=>CH3, CH3=>CH4, CH4=>BLANK, CH5=>BLANK, CH6=>BLANK, CH7=>CH0, SEL=>SEL, HEX=>HEX3);
	
	seg2: sevenSegmentDecoder_Mux
	port map(CH0=>CH2, CH1=>CH3, CH2=>CH4, CH3=>BLANK, CH4=>BLANK, CH5=>BLANK, CH6=>CH0, CH7=>CH1, SEL=>SEL, HEX=>HEX2);
	
	seg1: sevenSegmentDecoder_Mux
	port map(CH0=>CH3, CH1=>CH4, CH2=>BLANK, CH3=>BLANK, CH4=>BLANK, CH5=>CH0, CH6=>CH1, CH7=>CH2, SEL=>SEL, HEX=>HEX1);
	
	seg0: sevenSegmentDecoder_Mux
	port map(CH0=>CH4, CH1=>BLANK, CH2=>BLANK, CH3=>BLANK, CH4=>CH0, CH5=>CH1, CH6=>CH2, CH7=>CH3, SEL=>SEL, HEX=>HEX0);
end behavioral;