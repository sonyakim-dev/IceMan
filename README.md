# IceMan
In Iceman, the player has to dig underground tunnels through an oil field looking for oil. After the player has gathered all available oil within a particular field, the level is completed and the player advances to a new, more difficult oil field.<br/>
A band of Regular and Hardcore Protesters roam around the oil field and shout at the Iceman any time they get near to him. Each time the Iceman is shouted at, he becomes more irritated (and less healthy) and eventually he gives up and quits (loses a life). At this point, if the Iceman has more lives, the player can continue the game on the current level. If not, the game is over.

## How to start
- Mac OS X: You'll need to have <b>XQuartz</b> and <b>freeGLUT</b> installed to run the program<br/>
1. Install [XQuartz](http://xquartz.org/):<br/>
a. Click the download link, open the downloaded .dmg file, double-click on XQuartz.pkg, and follow the installation instructions.<br/>
b. Log out and log back in again.<br/>
c. To verify the installation, open a Terminal window and run the command `echo $DISPLAY`. That should produce one line of output that ends with `org.macosforge.xquartz:0`.

2. Install freeGLUT. You have two options:<br/>
a. Option 1: Install the [MacPorts](https://guide.macports.org/) package manager. Select the appropriate installation links and install the package.<br/>
b. Option 2: Install the [homebrew](http://brew.sh/) package manager. Then run the command `brew install homebrew/x11/freeglut`.

3. In terminal: `git clone https://github.com/sonyakim-dev/CS30-Project4-IceMan.git`
4. Open the project in Xcode and run it.

## Sample Screen
<img src="https://github.com/sonyakim-dev/CS30-Project4-IceMan/blob/main/iceman-sample.png?raw=true" width=500/>

## Video Demo
[Youtube Link](https://youtu.be/mTAIY-mC8tI)
