# Building

You need:

- Python
- Pipenv
- ee-gcc `2.96-ee-001003-1` (place it in `tools/cc/2.96-ee-001003-1`)
- The original Sly 2 ELF file

Make an `elf` directory in the root of the repository, and copy the original game ELF to `elf/sly2_[region].elf`.

Install python dependencies and enter the pipenv managed virtualenv:
```
pipenv install
pipenv shell
```

Build the patch:

```
make
```

# Running

Running PCSX2 with these flags allows running the patched elf with the original Sly 2 disc image:

`pcsx2 -elf elf/sly2_[region]_patched.elf -- /path/to/sly2_[region].iso`


# AI Policy

LLM generated code and pull requests which are obviously written by an agent will **not** be accepted.
