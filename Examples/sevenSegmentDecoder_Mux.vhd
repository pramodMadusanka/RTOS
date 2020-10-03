library ieee;
use ieee.std_logic_1164.all;

entity sevenSegmentDecoder_Mux is
	port(CH0,CH1,CH2,CH3,CH4,CH5,CH6,CH7: in std_logic_vector(2 downto 0);
		  SEL: in std_logic_vector(2 downto 0);
		  HEX: out std_logic_vector(0 to 6));
end sevenSegmentDecoder_Mux;

architecture behavioral of sevenSegmentDecoder_Mux is
	component mux_8_1_3bit
		port(S,T,U,V,W,X,Y,Z: in std_logic_vector(2 downto 0);
			  SEL: in std_logic_vector(2 downto 0);
			  M: out std_logic_vector(2 downto 0));
	end component;
	
	component sevenSegmentDecoder
		port(C: in std_logic_vector(2 downto 0);
			  HEX: out std_logic_vector(0 to 6));
	end component;
	
	signal muxOut: std_logic_vector(2 downto 0);

begin
	mux: mux_8_1_3bit
	port map(S=>CH0, T=>CH1, U=>CH2, V=>CH3,
			   W=>CH4, X=>CH5, Y=>CH6, Z=>CH7,
				SEL=>SEL, M=>muxOut);
	
	dec: sevenSegmentDecoder
	port map(HEX=>HEX, C=>muxOut);
end behavioral;