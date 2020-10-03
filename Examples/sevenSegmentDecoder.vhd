library ieee;
use ieee.std_logic_1164.all;

entity sevenSegmentDecoder is
	port(C: in std_logic_vector(2 downto 0);
		  HEX: out std_logic_vector(0 to 6));
end sevenSegmentDecoder;

architecture behavioral of sevenSegmentDecoder is
 begin
	HEX(0)<= not(not C(2) and C(0));
	HEX(1)<= not(not C(2) and ((not C(0) and not C(1)) or (C(0) and C(1))));
	HEX(2)<= not(not C(2) and ((not C(0) and not C(1)) or (C(0) and C(1))));
	HEX(3)<= not(not C(2) and (C(0) or C(1)));
	HEX(4)<= C(2);
	HEX(5)<= C(2);
	HEX(6)<= C(2) or C(1);
end behavioral;