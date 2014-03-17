Broadcast Address
My default broadcast address is in ¡°CommPoint.h¡±, on the begin of the file there is a variable ¡°const std::string defaultBroadCastAddr = ¡°10.0.2.255¡±;¡±

Compile:
1.There is a makefile "robot.mak" in the source code folder, just use "make -f robot.mak" to compile all the robots source code, then the executed file ¡°robot¡± will generated in bin\ folder.
2.There is a makefile "manager.mak" in the source code folder, just use "make -f manager.mak" to compile all the manager source code, then the executed file ¡°manager¡± will generated in bin\ folder.
3.In the makefile on the area commented as ¡°user-editable area¡±:
	a.player_inc is to indicate the player include file folder.
	b.player_lib is to indicate the player lib file folder.
	c.boost_dir is to indicate the boost include file folder.
	d.boost_lib is to indicate the boost lib file folder.
	e.If you do not have boost library in your system, here are my file:
		i.boost_dir: https://onedrive.live.com/redir?resid=D17BD252CC3EE29F!29567&authkey=!AOze4wGN8W4jaSo&ithint=folder%2c.hpp
		ii.boost_lib:  https://onedrive.live.com/redir?resid=D17BD252CC3EE29F!33588&authkey=!AKsS0n-01EqWspQ&ithint=folder%2c

Execute:
1.There are already two executable file ¡°robot¡± and "manager" in the bin\ folder, which can be executed on a 32bit Linux system. 
2.Execute robot program as ¡°./robot 6665 ¡± and ¡°./robot 666x¡±, 6665 is the leader and others are follower.
3.Or there is a script files ¡°createRobot.sh¡± and¡±create_Brobot.sh¡± in the ¡°MyRobot¡± folder, which can create the 4 robots one time.
4.Execute manager program as "./manger", then input the waypints, use "quit" to end the input.
5.Or Execute manger program as "./manger default", it will input the 3 default waypints for you.
