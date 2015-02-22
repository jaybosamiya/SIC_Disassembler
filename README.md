SIC Disassembler
================

Simplified Instruction Computer is an architecture created by L. L. Beck for his book on System Software. Due to its design, it is much easier to learn about Assemblers, Disassemblers, Linkers, Loaders, etc.

This project aims to create a program which takes an SIC object file and returns a corresponding assembly file which is equivalent to it.

Note: This software was created as an assignment/project as part of the System Software course at IIT Roorkee.

Instructions for Compilation
----------------------------

Compiling the disassembler follows the steps of standard CMake based build.

Basically, just run the following steps in the source directory:

```
mkdir bin
cd bin
cmake ..
make
```

This will create a `sic-disassemble` file which can then be used to disassemble SIC object code

Usage Syntax
------------

Running `./sic-disassemble` without any parameters shows the usage

```
Usage: ./sic-disassemble input_object_file output_assembly_file [-a]
  Options:
    -a   Show addresses in assembly output
```

Legal
-----

Please read the [LICENSE](LICENSE) file to know legal details of how you are allowed to use this software. But in short, this software is licensed under MIT License.

Disclaimer
----------

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.