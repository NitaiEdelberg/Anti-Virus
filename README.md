# Anti Virus

This program simulates an anti-virus that detects and neutralizes viruses within binary files. Based on the given known list of virus signatures, the program can automatically neutralize any viruses that infect your file!

## How to Use

1. **Memory Management and Debugging**:
   - Fix segmentation faults and memory leaks in a provided buggy program using `gdb` and `valgrind`.
   - Write `hexaPrint`, a program that receives a binary file and prints each byte in hexadecimal format.

2. **Virus Detector**:
   - Create a virus detection program that uses linked lists to store virus signatures.
   - Detect viruses by comparing bytes in a file to the virus signatures stored in the linked list.

3. **Virus Neutralization**:
   - Automatically neutralize viruses by modifying their code in the binary file.

## How to Run

1. **Compile the Program**:
   - Run the following in the shell to compile:
     ```bash
     make
     ```

2. **Run the Program**:
   - Use the following syntax to run the anti-virus program:
     ```bash
     ./AntiVirus FILE
     ```
   - `FILE`: The file to check for viruses.

3. **Program Menu**:
   - When the program runs, the following menu will appear:
     ```
     0) Set signatures file name
     1) Load signatures
     2) Print signatures
     3) Detect viruses
     4) Fix file
     5) Quit
     ```
     - **0) Set signatures file name**: Set the file containing virus signatures.
     - **1) Load signatures**: Load virus signatures from the current file.
     - **2) Print signatures**: Display all loaded virus signatures.
     - **3) Detect viruses**: Check the specified file for virus signatures.
     - **4) Fix file**: Neutralize detected viruses.
     - **5) Quit**: Exit the program.

### Example

```bash
make
./AntiVirus suspectedFile
