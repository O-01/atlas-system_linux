#include "hreadelf.h"

static void classer(unsigned char clas);
static void endianer(unsigned char dat);
static int section_header(elf_dt *elfen);

prog_dt prog;

/**
 * main - entry to program
 * @argc: count of arguments to program upon launch
 * @argv: vector of arguments to program upon launch
 * Return: 0 upon success
*/
int main(int argc, char **argv)
{
	size_t rd = 0;
	elf_dt elfen;

	memset(&elfen, 0, sizeof(elfen));
	prog.name = argv[0], prog.file = argv[1];
	if (argc == 2)
	{
		prog.fd = open(argv[1], O_RDONLY);
		if (prog.fd == -1)
			error_manager(1, 1);
		rd = read(prog.fd, &elfen.fh64, sizeof(elfen.fh64));
		if (rd != sizeof(elfen.fh64) || !is_elf((char *)&elfen.fh64))
			error_manager(2, 1);
		else
		{
			classer(elfen.fh64.e_ident[EI_CLASS]);
			if (prog.fclass == ELFCLASS32)
			{
				lseek(prog.fd, 0, SEEK_SET);
				rd = read(prog.fd, &elfen.fh32, sizeof(elfen.fh32));
				if (rd != sizeof(elfen.fh32) || !is_elf((char *)&elfen.fh32))
					error_manager(2, 1);
			}
			endianer(elfen.fh64.e_ident[EI_DATA]);
			if (!section_header(&elfen))
				error_manager(3, 1);
		}
	}
	else
		fprintf(stderr, "%s: Usage: %s ELF-file\n", prog.name, prog.name);
	if (elfen.sh32)
		free(elfen.sh32), elfen.sh32 = NULL;
	if (elfen.sh64)
		free(elfen.sh64), elfen.sh64 = NULL;
	return (0);
}

/**
 * classer - sets fclass value in program data struct
 * @clas: unsigned char representing file class of ELF file
*/
static void classer(unsigned char clas)
{
	if (clas == ELFCLASS32)
		prog.fclass = ELFCLASS32;
	else if (clas == ELFCLASS64)
		prog.fclass = ELFCLASS64;
}

/**
 * endianer - sets endianness value in program data struct
 * @dat: unsigned char representing endianness of ELF file
*/
static void endianer(unsigned char dat)
{
	if (dat == ELFDATA2LSB)
		prog.endianness = LSB;
	else if (dat == ELFDATA2MSB)
		prog.endianness = MSB;
}

/**
 * section_header - manages print of section header information from input file
 * @elfen: ELF file data struct
 * Return: 1 upon success, 0 if file is not an ELF file
*/
static int section_header(elf_dt *elfen)
{
	if (prog.fclass == ELFCLASS32)
	{
		if (prog.endianness == MSB)
			convert_fh_endian_32(&elfen->fh32);
		if (sh_details_32(elfen))
			return (0);
	}
	else if (prog.fclass == ELFCLASS64)
	{
		if (prog.endianness == MSB)
			convert_fh_endian_64(&elfen->fh64);
		if (sh_details_64(elfen))
			return (0);
	}
	return (1);
}

