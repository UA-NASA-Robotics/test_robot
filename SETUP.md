Hello future developer or user of the TESTEE code, I hope you are having a good day! Below is all the documentation that you should need to be able to start from scratch, as if you are a brand new member of the team, setting up TESTEE. There are obviosuly a lot of libraries, installations, or just information that should be documented so this is the place to put it, so if you discover something knew please be sure to add it to this pool of knowledge. This is also useful for if this code sees competition we know exactly what is needed to run. 

You should be using Ubuntu 24.04 as your primary OS for development, either on your primary computer or as a virtual machine. I am using Oracle VM Virtual Box with Noble downloaded and it works great so I recomend you use it to.

Once you get your operating system going, use Visial Studio Code as your IDE. It is easy to download as you just got to go linux app store and type in Code and download it from there.

Install git by running the command: sudo apt install git

Test_bot should be git cloned in the following manor; /home/YOURNAME/dev_ws/src/test_bot

Doanload the version of ROS2 that is supposed to be ran on your OS, for Noble its Jazzy Jalisco, and follow the instructions such as here; https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debians.html. Unless this changes, I recommend doing only the core packages and exclude the optional develeopment tools.

To not need to run "source /opt/ros/jazzy/setup.bash" at the begging of each new terminal to use ROS, run the following command once; echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc

The initial development of this code followed a youtube sereis and it is stronlgy recommendted it should be watched to understand the fundamentials of ros, the basics of ros development, and important commands to make this codebase work. https://www.youtube.com/watch?v=OWeLUSzxMsw&list=PLunhqkrRNRhYAffV8JDiFOatQXuU-NnxT

For anything you sudo apt install you should place the name of the package within the packages.txt file. Make sure you put only the name of the packages with no spaces and that anything you download works with the version of ros in use. This will be the baseline of how we ensure everyone has the right packages to develope. To sudo apt install the entire list, navigate into the test_bot folder in a terminal and run: sudo apt install $(cat packages.txt)