#MPFA-ARGoS with static/dynamic depots and MPFA with a hirarchical branching transportation network 

MPFA_H: The MPFA with a hirarchical branching transportation network. See the video https://youtu.be/tgSTCz264cU 

Dynamic_MPFA: The MPFA with dynamic depots. Depots moves based on the discovered local information. See the video https://www.youtube.com/watch?v=vu7QXRFIlj8&list=PLkjRv85y76xlLHEr0ekXVnVTy_z9TMZD4

Static_MPFA: The MPFA with static depots. Depots are distributed uniformly and do not move. 

Global_dynamic_MPFA: The MPFA with dynamic depots. We assume the locations of all resources are known. Depots moves based on the locations of resources in the arena. They always move to the center of the locations of closest resources. 

Global_static_MPFA: The MPFA with static depots. We assume the locations of all resources are known. Depots are distributed to the center of the locations of closest resources and will not move in the whole process of foraging. 


Quick Start Installation Guide

The CPFA and MPFA system have two components: the CPFA and MPFA logic controllers that implement the CPFA and MPFA algorithm in the ARGoS robot simulator, and the Genetic Algorithm that evolves the parameters that the algorithms use. You can run the algorithm on ARGoS using OS X or Linux (see installation instructions below). 

In order to use the CPFA and MPFA in ARGoS, you must first install ARGoS on your system then download and compile the code in this repo to run with ARGoS.

1. Installing ARGoS

ARGoS is available for Windows, Linux and Macintosh systems. Detailed installation instructions can be found on the ARGoS Website (http://www.argos-sim.info/core.php).

Linux Installation

Download the appropriate binary package for your Linux system.
In Terminal, run the following command in the directory of your installation file:

for Ubuntu and KUbuntu:

$ sudo dpkg -i argos3_simulator-*.deb
for OpenSuse:

$ sudo rpm -i argos3_simulator-*.rpm
Macintosh Installation

The Mac OSX installation of ARGoS uses the Homebrew Package Manager. If you don't have it, install Homebrew by using the following command in Terminal.

$ ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
Obtain the Homebrew Tap for ARGoS using the following command in Terminal.

$ brew tap ilpincy/argos3
Once tapped, install ARGoS with the following command in Terminal. ARGoS and its required dependencies will be downloaded and installed using Homebrew.

$ brew install bash-completion qt lua argos3
Once installed, you can update ARGoS with the following two commands in Terminal.

$ brew update
$ brew upgrade argos3
2. Compiling and Running the CPFA in ARGoS

Once ARGoS is installed on your system. You can download the files in this repo, compile them for your system, and run the iAnt CPFA in ARGoS.

Pull the code from this repository.

From the terminal, use build.sh script to compile the code:

$ ./build.sh
CPFA-ARGoS includes CPFA evolver. This program uses a distributed version of ga-lib to evolve CPFA parameters. An example script for running cpfa_evolver is provided: evolve_EXAMPLE.sh.

CPFA evolver uses MPI to distribute argos evaluations across a cluster. An example machine file (moses_cluster) specifies the hostnames of the MPI nodes to use and the number of processes to run on each node.

evolve_EXAMPLE.sh takes two arguments. The number of MPI processes to run and the machine file name for the MPI cluster.

Since the evolver relies on MPI packages that are not required for compiling the CPFA, compilation of the evolver is turned off by default.

To build the CPFA evolver modify the build.sh script and change

cmake -DBUILD_EVOLVER=NO ..
to

cmake -DBUILD_EVOLVER=YES ..
The evolver takes an experiment xml file argument that specifies the simulation parameters. The CPFA genome in that experient file is ignored and evolved parameters used instead. Make sure visualisation is turned off in this experiment file.

3. Running an Experiment

To run an experiment launch ARGoS with the XML configuration file for your system:

  $ argos3 -c experiments/experiment_file.xml
