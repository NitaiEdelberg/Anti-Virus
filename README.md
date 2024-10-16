# Anti-Virus

This program simulates an anti-virus system designed to detect and neutralize viruses in binary files. By using a known list of virus signatures, the program can automatically identify and neutralize any viruses within your files.

## Features

- **Virus Detection**: Scans binary files for known virus signatures.
- **Virus Neutralization**: Automatically removes or neutralizes detected viruses by modifying infected code within the file.
- **Signature Management**: Handles virus signatures using a linked list structure for efficient storage and lookup.

## How to Use

1. **Upload a Signatures File and an Infected File**:
   - The virus signatures are stored in the background using a linked list structure.
   - The infected file will be checked against these stored virus signatures.

2. **Virus Detection**:
   - Detects viruses by comparing the bytes in the file to the signatures loaded into the linked list.

3. **Virus Neutralization**:
   - Neutralizes viruses by modifying the infected portions of the binary file based on the detected signatures.

## How to Run

1. **Compile the Program**:
   - Run the following command in the terminal to compile the program:
     ```bash
     make
     ```

2. **Execute the Program**:
   - Use the following syntax to run the anti-virus program:
     ```bash
     ./AntiVirus <FILE>
     ```
   - `FILE`: The name of the binary file to scan for viruses.

3. **Interactive Program Menu**:
   - After starting the program, the following menu options will appear:
     ```
     0) Set signatures file name
     1) Load signatures
     2) Print signatures
     3) Detect viruses
     4) Fix file
     5) Quit
     ```
     - **0) Set signatures file name**: Set the file that contains virus signatures.
     - **1) Load signatures**: Load virus signatures from the specified file into memory.
     - **2) Print signatures**: Display all currently loaded virus signatures.
     - **3) Detect viruses**: Scan the specified binary file for viruses.
     - **4) Fix file**: Neutralize detected viruses in the file.
     - **5) Quit**: Exit the program.



 ## Example Usage

```bash
# Compile the program
make

# Run the anti-virus on an infected file
./AntiVirus infected

# set signatures file name
0
signatures-B

# Load signatures from a file
1

# Detect and neutralize viruses
3
4
quit
