# CppND-System-Monitor

![System Monitor](images/monitor.png)



## Dependencies
* ncurses  
    [ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for displaying output.

    If you are not using the Workspace, install ncurses within your own Linux environment: 
    ```bash
    sudo apt install libncurses5-dev libncursesw5-dev
    ```

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the repository

2. Build the project: 
    ```bash
    make build
    ```

3. Run the resulting executable: 
    ```bash
    ./build/monitor
    ```
    ![Starting System Monitor](images/starting_monitor.png)
