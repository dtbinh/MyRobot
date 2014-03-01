Broadcast Address
My default broadcast address is in ¡°CommPoint.h¡±, on the begin of the file there is a variable ¡°const std::string defaultBroadCastAddr = ¡°10.0.2.255¡±;¡±

Random the world:
1.The world file I used is ¡°empty.cfg¡± and ¡°empty.world¡±.
2.There are a script file ¡°create_world.sh¡± and a python file ¡°posegen.py¡± that can generate random robot location in ¡°empty.world¡± (Thanks for Nick¡¯s great job).
3.You may want to execute ¡°create_world.sh¡± every time you need a new robot distribution. 
4.Also you may need to modify the python interpreter location on the first line of ¡°posegen.py¡±.

Compile:
1.There is a default makefile in the source code folder, just use "make" to compile all the source code, then the executed file ¡°robot¡± will generated in bin\ folder.
2.In the makefile on the area commented as ¡°user-editable area¡±:
	a.player_inc is to indicate the player include file folder.
	b.player_lib is to indicate the player lib file folder.
	c.boost_dir is to indicate the boost include file folder.
	d.boost_lib is to indicate the boost lib file folder.
	e.If you do not have boost library in your system, here are my file:
		i.boost_dir: https://onedrive.live.com/redir?resid=D17BD252CC3EE29F!29567&authkey=!AOze4wGN8W4jaSo&ithint=folder%2c.hpp
		ii.boost_lib:  https://onedrive.live.com/redir?resid=D17BD252CC3EE29F!33588&authkey=!AKsS0n-01EqWspQ&ithint=folder%2c

Execute:
1.There is already an executable file ¡°robot¡± in the bin\ folder, which can be executed on a 32bit Linux system. 
2.Execute program as ¡°./robot 6665 a 140 50¡± and ¡°./robot 6665 d 140 50¡±.
3.Or there are two script files ¡°create_Arobot.sh¡± and¡±create_Brobot.sh¡± in the ¡°MyRobot¡± folder, which can create 6 aggregation or dispersion robots one time.
