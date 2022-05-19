/*
 * bfd.c
 *
 *  Created on: Dec 20, 2020
 *      Author: reiner
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfd.h>
#include "elf-bfd.h"

static bfd_vma pc;
static const char *g_filename;
static const char *g_functionname;
static unsigned int g_line;
static unsigned int g_discriminator;
static bfd_boolean found;

static asymbol **syms;		/* Symbol table.  */

/* Read in the symbol table.  */

static void slurp_symtab(bfd *abfd) {
	long storage;
	long symcount;
	bfd_boolean dynamic = FALSE;

	fprintf(stderr,"symtab\n");

	if ((bfd_get_file_flags(abfd) & HAS_SYMS) == 0)
		return;

	fprintf(stderr,"HU\n");

	storage = bfd_get_symtab_upper_bound(abfd);
	if (storage == 0) {
		storage = bfd_get_dynamic_symtab_upper_bound(abfd);
		dynamic = TRUE;
	}
	if (storage < 0) {
		// bfd_fatal(bfd_get_filename(abfd));
		fprintf(stderr,"Cannot process %s\n", g_filename);
		exit(1);
	}

	syms = (asymbol**) malloc(storage);
	if (dynamic)
		symcount = bfd_canonicalize_dynamic_symtab(abfd, syms);
	else
		symcount = bfd_canonicalize_symtab(abfd, syms);
	if (symcount < 0) {
		fprintf(stderr,"No symbols! Cannot process %s\n", g_filename);
		exit(1);
	}

	fprintf(stderr,"Symcount %ld %ld\n",symcount, storage);

	/* If there are no symbols left after canonicalization and
	 we have not tried the dynamic symbols then give them a go.  */
	if (symcount == 0 && !dynamic && (storage =
			bfd_get_dynamic_symtab_upper_bound(abfd)) > 0) {
		free(syms);
		syms = malloc(storage);
		symcount = bfd_canonicalize_dynamic_symtab(abfd, syms);
	}

	/* PR 17512: file: 2a1d3b5b.
	 Do not pretend that we have some symbols when we don't.  */
	if (symcount <= 0) {
		free(syms);
		syms = NULL;
	}
}

/* Look for an address in a section.  This is called via
   bfd_map_over_sections.  */
static void find_address_in_section (bfd *abfd, asection *section,
			 void *data) {
  bfd_vma vma;
  bfd_size_type size;

  if (found)
    return;

  if ((bfd_section_flags (section) & SEC_ALLOC) == 0)
    return;

  vma = bfd_section_vma (section);
  if (pc < vma)
    return;

  size = bfd_section_size (section);
  if (pc >= vma + size)
    return;

  found = bfd_find_nearest_line_discriminator (abfd, section, syms, pc - vma,
                                               &g_filename, &g_functionname,
                                               &g_line, &g_discriminator);
}


int main(int argc, char **argv) {
	const char *target = NULL;
	char *filename = "/home/reiner/Projects/OceanDSL/case-studies/MITgcm/verification/tutorial_barotropic_gyre/build/mitgcmuv";

	g_filename = filename;

	target = "x86_64-pc-linux-gnu";

	bfd_set_error_program_name(filename);

	if (bfd_init () != BFD_INIT_MAGIC) {
		fprintf(stderr,"init error");
	}

	bfd_set_default_target(target);

	bfd *abfd = bfd_openr(filename, target);
	if (abfd == NULL) {
		fprintf(stderr,"Cannot handle file %s\n", filename);
		exit(1);
	}

	abfd->flags |= BFD_DECOMPRESS;

	if (bfd_check_format (abfd, bfd_archive)) {
		fprintf(stderr,"format error\n");
	} else {
		fprintf(stderr,"Format good\n");
	}

    char **matching;

	if (! bfd_check_format_matches (abfd, bfd_object, &matching)) {
		fprintf(stderr,"Format check error\n");
		if (bfd_get_error () == bfd_error_file_ambiguously_recognized) {
		  int i;
		  for (i=0; matching[i] != NULL;i++) {
			  fprintf(stderr,">> %s\n", matching[i]);
		  }
		  free (matching);
		}
	    exit (1);
	}

	slurp_symtab(abfd);

	FILE* in = fopen("/home/reiner/temp/kieker-mitgcm/a", "r");
	char* line;
	char buffer[1024];
	while ((line=fgets(buffer, 1023, in)) != NULL) {
		found = 0;
		line[strlen(line)-1]=0;
		const char *addr_hex = line;

		pc = bfd_scan_vma(addr_hex, NULL, 16);
		// bfd_target_elf_flavour is from enum bfd_flavour
		if (bfd_get_flavour(abfd) == bfd_target_elf_flavour) {
			printf("Flavor ELF\n");
			const struct elf_backend_data *bed = abfd->xvec->backend_data;
			bfd_vma sign = (bfd_vma) 1 << (bed->s->arch_size - 1);

			pc &= (sign << 1) - 1;
			if (bed->sign_extend_vma)
				pc = (pc ^ sign) - sign;
		} else {
			printf("Flavor %d\n", bfd_get_flavour(abfd));
		}

		printf("print pc 0x ");
		bfd_printf_vma (abfd, pc);
		printf ("\n");

		asection* p;
		for (p = abfd->sections; p != NULL; p = p->next) {
			fprintf(stderr,"Hu %p\n",p);
		}

		bfd_map_over_sections(abfd, find_address_in_section, NULL);

		printf("name %s\n", g_functionname);
		printf("file %s\n", g_filename);
		printf("line %d\n", g_line);
	}
	fclose(in);
	bfd_close(abfd);
}

