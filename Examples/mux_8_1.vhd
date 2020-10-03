library ieee;
use ieee.std_logic_1164.all;

entity mux_8_1 is
	port(S,T,U,V,W,X,Y,Z: in std_logic;
		  SEL: in std_logic_vector(2 downto 0);
		  M: out std_logic);
end mux_8_1;

architecture behavior of mux_8_1 is
	component mux_2_1
		port(x,y,s: in std_logic;
			  m: out std_logic);
	end component;
	
	signal m0, m1, m2, m3, m4, m5: std_logic;
	
begin
	mux0: mux_2_1
	port map(x=>S, y=>T, s=>SEL(0), m=>m0);
	
	mux1: mux_2_1
	port map(x=>U, y=>V, s=>SEL(0), m=>m1);
	
	mux2: mux_2_1
	port map(x=>W, y=>X, s=>SEL(0), m=>m2);
	
	mux3: mux_2_1
	port map(x=>Y, y=>Z, s=>SEL(0), m=>m3);
	
	mux4: mux_2_1
	port map(x=>m0, y=>m1, s=>SEL(1), m=>m4);
	
	mux5: mux_2_1
	port map(x=>m2, y=>m3, s=>SEL(1), m=>m5);
	
	mux6: mux_2_1
	port map(x=>m4, y=>m5, s=>SEL(2), m=>M);
end behavior;	
