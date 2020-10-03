library ieee;
use ieee.std_logic_1164.all;

entity mux_8_1_3bit is
	port(S,T,U,V,W,X,Y,Z: in std_logic_vector(2 downto 0);
		  SEL: in std_logic_vector(2 downto 0);
		  M: out std_logic_vector(2 downto 0));
end mux_8_1_3bit;

architecture behavioral of mux_8_1_3bit is
	component mux_8_1
		port(S,T,U,V,W,X,Y,Z: in std_logic;
		  SEL: in std_logic_vector(2 downto 0);
		  M: out std_logic);
	end component;
	
begin	
	mux0: mux_8_1
	port map(S=>S(0), T=>T(0), U=>U(0), V=>V(0), W=>W(0), X=>X(0), Y=>Y(0), Z=>Z(0), SEL=>SEL, M=>M(0));
	
	mux1: mux_8_1
	port map(S=>S(1), T=>T(1), U=>U(1), V=>V(1), W=>W(1), X=>X(1), Y=>Y(1), Z=>Z(1), SEL=>SEL, M=>M(1));
	
	mux2: mux_8_1
	port map(S=>S(2), T=>T(2), U=>U(2), V=>V(2), W=>W(2), X=>X(2), Y=>Y(2), Z=>Z(2), SEL=>SEL, M=>M(2));
end behavioral;