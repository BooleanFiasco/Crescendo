# Crescendo
Procedural roguelike with a 3D isometric perspective, a unique combat system and intuitive touch controls.

## Cloning the Crescendo repository (for programmers):
1. Install the most recent version of Git-SCM from here: https://git-scm.com/
2. Next, install the most recent version of Git-LFS from here: https://git-lfs.github.com/
  * Remember to run the command `git install lfs` as directed in the instructions on that page!
3. Open a Git Bash terminal and navigate to the directory you want to store your repositories in. *Do not* create your project directory yet - that will happen in the next step.
4. Run `git clone --recursive https://github.com/BooleanFiasco/Crescendo.git` and wait for the project to finish copying.
5. Switch into the Crescendo/UnrealEngine directory and run Setup.bat to download and install UE4 dependencies and base content.
6. Run CreateProjectFiles.bat
7. Switch back to the Crescendo directory, right-click on the Crescendo.uproject file and choose "Create project files."
8. Open Crescendo.sln and build!

## Cloning the Crescendo repository (for artists):
TODO

## Getting the Crescendo engine build (for artists):
The latest engine build can be found on our Google drive: [Crescendo/Engine Builds](https://drive.google.com/open?id=0B2s873J_IiIEV1FmVUgyUk95azQ)

Build archives names are standardized around when they were made - the last 8 characters of the name will be the date that the build was created in YYYYMMDD format. Thus, the most recent build will always be the *last* one listed in the folder (assuming there is more than one build).

To use the build, simply download it from the Google drive to your Crescendo project folder. It should look something like this:

Next, use 7-Zip to extract the archive in-place. Just right-click on it and choose 7-Zip > Extract Here. When it asks if you would like to replace files, click Yes.

Once the extraction is complete, you may need to associate the project with this engine installation. To do so, right-click on the Crescendo.uproject file and select "Switch Unreal Engine version...", then choose the installation in your project directory. You should only need to do this once - for subsequent engine updates it shoud not be necessary.

## Setup UE4 Git source control plugin
TODO: describe this process
