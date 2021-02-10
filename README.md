# SIC/XE Assembler:
Its a basic SIC/XE Assembler written using C++. Might port to golang in the future. Also might build an emulator to run the generated object program from here.

### Run :
- Run a test on some test inputs
    ```bash
    $ make test
    ```
    If you have valgrind you can check for memory leaks with
    ```
    $ make memtest
    ```
- Build the compiled source of the project
    ```bash
    $ make build
    ```
 
    Build outputs to a folder named bin with the executable.
    
- Run the code with
    ```bash
    $ app.exe path/to/input/file
    ```
