
   /*+------- <| --------------------------------------------------------**
    **         A                     Clan                                **
    **---     /.\   -----------------------------------------------------**
    **   <|  [""M#                 symbol.c                              **
    **-   A   | #   -----------------------------------------------------**
    **   /.\ [""M#         First version: 01/05/2008                     **
    **- [""M# | #  U"U#U  -----------------------------------------------**
         | #  | #  \ .:/
         | #  | #___| #
 ******  | "--'     .-"  ******************************************************
 *     |"-"-"-"-"-#-#-##   Clan : the Chunky Loop Analyzer (experimental)     *
 ****  |     # ## ######  *****************************************************
 *      \       .::::'/                                                       *
 *       \      ::::'/     Copyright (C) 2008 University Paris-Sud 11         *
 *     :8a|    # # ##                                                         *
 *     ::88a      ###      This is free software; you can redistribute it     *
 *    ::::888a  8a ##::.   and/or modify it under the terms of the GNU Lesser *
 *  ::::::::888a88a[]:::   General Public License as published by the Free    *
 *::8:::::::::SUNDOGa8a::. Software Foundation, either version 2.1 of the     *
 *::::::::8::::888:Y8888:: License, or (at your option) any later version.    *
 *::::':::88::::888::Y88a::::::::::::...                                      *
 *::'::..    .   .....   ..   ...  .                                          *
 * This software is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   *
 * for more details.							      *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with software; if not, write to the Free Software Foundation, Inc.,  *
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA                     *
 *                                                                            *
 * Clan, the Chunky Loop Analyzer                                             *
 * Written by Cedric Bastoul, Cedric.Bastoul@u-psud.fr                        *
 *                                                                            *
 ******************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include <osl/macros.h>
#include <osl/strings.h>
#include <osl/generic.h>
#include <osl/relation.h>
#include <osl/relation_list.h>
#include <osl/extensions/arrays.h>
#include <osl/extensions/symbols.h>
#include <osl/extensions/symbol_rank.h>
#include <clan/macros.h>
#include <clan/relation.h>
#include <clan/symbol.h>


void yyerror(char*);


/*+****************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/


/*+****************************************************************************
 *                            Processing functions                            *
 ******************************************************************************/


/**
 * clan_symbol_get_identifier function:
 * This function returns the textual identifier associated with the symbol
 * provided as parameter. It returns NULL if unsuccessful.
 * \param[in] symbol     Pointer to symbols whose identifier is needed.
 * \return Pointer to the textual identifier
 */
char * clan_symbol_get_identifier(osl_symbols_p symbol) {
  if(symbol==NULL || symbol->identifier==NULL)
    return NULL;
  osl_strings_p identifier = (osl_strings_p)symbol->identifier->data; 
  assert(osl_strings_size(identifier)==1);
  return identifier->string[0];
}

/**
 * clan_symbol_lookup function:
 * This function searches the symbol table for a symbol with the identifier
 * provided as parameter. It returns the pointer to the symbol if it already
 * exists inside the table, NULL otherwise.
 * \param[in] symbol     The first node of the list of symbols.
 * \param[in] identifier The identifier we are looking for.
 * \return The symbol corresponding to identifier, NULL if it doesn't exist.
 */
osl_symbols_p clan_symbol_lookup(osl_symbols_p symbol, char* identifier) {
  char *sym_identifier = NULL;
  while (symbol != NULL) {
    sym_identifier = clan_symbol_get_identifier(symbol);
    if (strcmp(sym_identifier, identifier) == 0)
      return symbol;
    else
      symbol = symbol->next;
  }
  return NULL;
}

  
/**
 * clan_symbol_lookup_by_key function:
 * This function searches the symbol table for a symbol with the key
 * provided as parameter. It returns the pointer to the symbol if it already
 * exists inside the table, NULL otherwise.
 * \param[in] symbol The first node of the list of symbols.
 * \param[in] key    The key of the searched symbol.
 * \return The symbol corresponding to the key, or NULL if it doesn't exist.
 */
osl_symbols_p clan_symbol_lookup_by_key(osl_symbols_p symbol, int key) {
  while (symbol != NULL) {
    if (symbol->tag == key)
      return symbol;
    else
      symbol = symbol->next;
  }
  return NULL;
}


/**
 * clan_symbol_generate_new_key function:
 * this function generates a key which is not yet present in the table.
 * \param[in] table The first element of the symbol table.
 * \return A key which would be convenient for a new symbol.
 */
static
int clan_symbol_generate_new_key(osl_symbols_p table) {
  int key = CLAN_KEY_START;
  
  while (table != NULL) {
    if (table->tag >= key)
      key = table->tag + 1;
    table = table->next;
  }
  return key;
}


/**
 * clan_symbol_add function:
 * This function adds a new osl_symbol_t in the symbol table whose address
 * is provided as a parameter. If the symbol table is empty (NULL), the new
 * node will become its first element. A new node is added only if an
 * existing node with the same identifier does not already exist. It returns
 * the pointer to the symbol table node corresponding to the identifier.
 * \param[in,out] table      The address of the symbol table.
 * \param[in]     identifier The identifier of the symbol we want to add.
 * \param[in]     type       The new symbol type.
 * \return                   Pointer to the found or added symbol.
 */
osl_symbols_p clan_symbol_add(osl_symbols_p* table, char* identifier,
                              int type) {
  osl_symbols_p symbol, tmp = *table;
  osl_strings_p strings = NULL;

  // If the identifier is already in the table, do nothing.
  symbol = clan_symbol_lookup(*table, identifier);
  if (symbol != NULL)
    return symbol;

  // Else, we allocate and fill a new osl_symbol_t node.
  symbol = osl_symbols_malloc();
  symbol->tag = clan_symbol_generate_new_key(*table);
  strings = osl_strings_encapsulate(strdup(identifier));
  symbol->identifier = osl_generic_shell(strings, osl_strings_interface());
  symbol->type = type;

  // We put the new symbol at the end of the table.
  if (*table == NULL) {
    *table = symbol;
  } else {
    while (tmp->next != NULL)
      tmp = tmp->next;
    tmp->next = symbol;
  }

  return symbol;
}


/**
 * clan_symbol_get_key function:
 * This function returns the key of the symbol with identifier "identifier"
 * in the symbol table whose first element is "symbol". If the symbol with
 * the specified identifier is not found, it returns CLAN_UNDEFINED.
 * \param[in] symbol     The first node of the list of symbols.
 * \param[in] identifier The identifier we want to know the key.
 * \return The key corresponding to the identifier or CLAN_UNDEFINED.
 */
int clan_symbol_get_key(osl_symbols_p symbol, char* identifier) {
  char *sym_identifier = NULL;
  while (symbol != NULL) {
    sym_identifier = clan_symbol_get_identifier(symbol);
    if (strcmp(sym_identifier,identifier) == 0)
      return symbol->tag;
    else
      symbol = symbol->next;
  }
  return CLAN_UNDEFINED;
}


/**
 * clan_symbol_get_rank function:
 * This function returns the rank of the symbol whose pointer is passed.
 * \param[in] symbol     The node whose rank is requested.
 * \return The rank corresponding to the symbol or CLAN_UNDEFINED.
 */
int clan_symbol_get_rank(osl_symbols_p symbol) {
  osl_symbol_rank_p srank = NULL;
  if (symbol != NULL && symbol->extent != NULL) {
    srank = (osl_symbol_rank_p)symbol->extent->data;
    return osl_symbol_rank_get_rank(srank);
  }
  return CLAN_UNDEFINED;
}

/**
 * clan_symbol_set_rank function:
 * This function modifies the rank of the symbol whose pointer is passed.
 * \param[in] symbol     The node whose rank is to be modified.
 * \param[in] rank       The new value for the rank.
 * \return The rank corresponding to the symbol or CLAN_UNDEFINED.
 */
void clan_symbol_set_rank(osl_symbols_p symbol, int rank) {
  osl_symbol_rank_p srank = NULL;
  if (symbol != NULL) {
    if (symbol->extent != NULL) {
      srank = (osl_symbol_rank_p)symbol->extent->data;
      osl_symbol_rank_set_rank(srank, rank);
    }
    else{
      osl_symbol_rank_p tmp = osl_symbol_rank_malloc();
      tmp->rank = rank;
      symbol->extent = osl_generic_shell(tmp, osl_symbol_rank_interface());
    }
  }
}


/**
 * clan_symbol_get_type function:
 * This function returns the type of the symbol with identifier "identifier"
 * in the symbol table whose first element is "symbol". If the symbol with
 * the specified identifier is not found, it returns -1.
 * \param[in] symbol     The first node of the list of symbols.
 * \param[in] identifier The identifier we want to know the type.
 * \return The type of the symbol corresponding to the identifier.
 */
int clan_symbol_get_type(osl_symbols_p symbol, char* identifier) {
  char *sym_identifier = NULL;
  while (symbol != NULL) {
    sym_identifier = clan_symbol_get_identifier(symbol);
    if (strcmp(sym_identifier,identifier) == 0)
      return symbol->type;
    else
      symbol = symbol->next;
  }
  return CLAN_UNDEFINED;
}


/**
 * clan_symbol_array_to_strings function:
 * this functions builds (and returns a pointer to) an osl_strings_t
 * structure containing the symbol strings contained in an array of
 * symbols of length size. The symbol string order is the same as the one
 * in the symbol array.
 * \param[in] sarray The symbol array.
 * \param[in] size   The size of the symbol array.
 * \return An osl_strings_t containing all the symbol strings.
 */
osl_strings_p clan_symbol_array_to_strings(osl_symbols_p* sarray, int size) {
  int i, length;
  char** identifiers = NULL;
  char*  identifier  = NULL;
  osl_strings_p strings;

  // Allocate, initialize and NULL-terminate the array of strings.
  CLAN_malloc(identifiers, char**, (size + 1) * sizeof(char *));
  for (i = 0; i <= size; i++)
    identifiers[i] = NULL;

  // Fill the array of strings.
  for (i = 0; i < size; i++) {
    identifier = clan_symbol_get_identifier(sarray[i]);
    length = strlen(identifier) + 1;
    CLAN_malloc(identifiers[i], char*, length * sizeof(char));
    strcpy(identifiers[i], identifier);
  }

  // Build the osl_strings_t container.
  strings = osl_strings_malloc();
  strings->string = identifiers;

  return strings;
}


/**
 * clan_symbol_nb_of_type function:
 * this function returns the number of symbols of a given type in the
 * symbol table.
 * \param[in] symbol The top of the symbol table.
 * \param[in] type   The type of the elements.
 * \return The number of symbols of the provided type in the symbol table.
 */
int clan_symbol_nb_of_type(osl_symbols_p symbol, int type) {
  int nb = 0;
  
  while (symbol != NULL) {
    if (symbol->type == type)
      nb++;
    symbol = symbol->next;
  }

  return nb;
}


/**
 * clan_symbol_to_strings function:
 * this function builds (and returns a pointer to) an osl_generic_t
 * structure containing the symbol strings of a given type in the
 * symbol table. The osl_generic_t is a shell for an osl_strings_t
 * which actually stores the symbol strings. The symbol strings are sorted
 * in the same order as they appear in the symbol table. If there is no
 * corresponding symbol in the table, it returns NULL.
 * \param[in] symbol The top of the symbol table.
 * \param[in] type   The type of the elements.
 * \return An osl_generic_t with the symbol strings of the given type.
 */
osl_generic_p clan_symbol_to_strings(osl_symbols_p symbol, int type) {
  int i, length, nb_identifiers = 0;
  char** identifiers = NULL;
  char*  identifier  = NULL;
  osl_strings_p strings;
  osl_generic_p generic;

  nb_identifiers = clan_symbol_nb_of_type(symbol, type);
  if (nb_identifiers == 0)
    return NULL;

  // Allocate, initialize and NULL-terminate the array.
  CLAN_malloc(identifiers, char**, (nb_identifiers + 1) * sizeof(char *));
  for (i = 0; i <= nb_identifiers; i++)
    identifiers[i] = NULL;

  // We scan the table a second time to fill the identifier array
  // Not optimal to act this way but overkills are worse!
  i = 0;
  while (symbol != NULL) {
    if (symbol->type == type) {
      identifier = clan_symbol_get_identifier(symbol);
      length = strlen(identifier) + 1;
      CLAN_malloc(identifiers[i], char*, length * sizeof(char));
      strcpy(identifiers[i], identifier);
      i++;
    }
    symbol = symbol->next;
  }

  // Build the osl_strings_t container.
  strings = osl_strings_malloc();
  strings->string = identifiers;

  // Embed the strings in a generic shell.
  generic = osl_generic_shell(strings, osl_strings_interface());
  return generic;
}


/**
 * clan_symbol_to_arrays function:
 * this function generates an arrays extension from the symbol table
 * passed as an argument. It embeds it in an osl_generic_t structure
 * before returning it.
 * \param[in] symbol The symbol table.
 * \return An arrays structure with all the arrays of the symbol table.
 */
osl_generic_p clan_symbol_to_arrays(osl_symbols_p symbol) {
  int i;
  int nb_arrays = 0;
  osl_arrays_p arrays = NULL;
  osl_generic_p generic = NULL;
  osl_symbols_p top = symbol;

  // A first scan to know how many arrays there are.
  while (symbol != NULL) {
    nb_arrays++;
    symbol = symbol->next;
  }

  // Build the arrays extension.
  if (nb_arrays > 0) {
    arrays = osl_arrays_malloc();
    CLAN_malloc(arrays->id, int*, nb_arrays * sizeof(int));
    CLAN_malloc(arrays->names, char**, nb_arrays * sizeof(char *));
    arrays->nb_names = nb_arrays;
    symbol = top;
    i = 0;
    while (symbol != NULL) {
      arrays->id[i] = symbol->tag;
      CLAN_strdup(arrays->names[i], clan_symbol_get_identifier(symbol));
      i++;
      symbol = symbol->next;
    }
    
    // Embed the arrays in a generic shell.
    generic = osl_generic_shell(arrays, osl_arrays_interface());
  }

  return generic;
}


/**
 * clan_symbol_new_iterator function:
 * this function returns 1 if it succeeds to register (or to update) an
 * iterator in the symbol table and to add it to the iterator array. It
 * returns 0 otherwise. The reason for failure can be that the symbol
 * is already in use for something else than an iterator.
 * \param[in,out] table The symbol table.
 * \param[in,out] array The iterator array.
 * \param[in]     id    The textual name of the iterator.
 * \param[in]     depth The current loop depth.
 * \return 1 on success, 0 on failure.
 */
int clan_symbol_new_iterator(osl_symbols_p* table, osl_symbols_p* array,
                             char* id, int depth) {
  osl_symbols_p symbol;
  symbol = clan_symbol_add(table, id, OSL_SYMBOL_TYPE_ITERATOR);

  // Ensure that the returned symbol was either a new one, or of the same type.
  if (symbol->type != OSL_SYMBOL_TYPE_ITERATOR) {
    yyerror("a loop iterator was previously used for something else");
    return 0;
  }
  
  // Update the rank, in case the symbol already exists.
  if (clan_symbol_get_rank(symbol) != depth + 1)
    clan_symbol_set_rank(symbol, depth + 1);

  array[depth] = osl_symbols_nclone(symbol, 1);
  return 1;
}


/**
 * clan_symbol_update_type function:
 * this function returns 1 if it succeeds to modify the type of a symbol
 * in the symbol table. The modified symbol corresponds to the reference
 * accessed in the last access relation of an access list provided as
 * parameter. It returns 0 if it fails. The reasons for a failure can be
 * either to try to modify an iterator type or a parameter type since they
 * are supposed to be dead-ends.
 * \param[in,out] table  The first symbol of the symbol table (one element
 *                       may be modified).
 * \param[in]     access A list of access relations.
 * \param[in]     type   The new type for the symbol we want to update.
 * \return 1 on success, 0 on failure.
 */
int clan_symbol_update_type(osl_symbols_p table, osl_relation_list_p access,
                            int type) {
  int key;
  int relation_type;
  osl_symbols_p symbol;
  osl_generic_p gen;
  
  if (table == NULL)
    CLAN_error("cannot even try to update type: NULL symbol table");
  if (access == NULL)
    CLAN_error("cannot even try to update type: NULL access list");

  // We will only consider the last reference in the list.
  while (access->next != NULL)
    access = access->next;

  // Get the key (with some cheating with the relation type to be able to use
  // osl_relation_get_array_id), find the corresponding symbol and update.
  relation_type = access->elt->type;
  access->elt->type = OSL_TYPE_READ;
  key = osl_relation_get_array_id(access->elt);
  access->elt->type = relation_type;
  symbol = clan_symbol_lookup_by_key(table, key);
  if (symbol == NULL)
    CLAN_error("no symbol corresponding to the key");

  if ((symbol->type == OSL_SYMBOL_TYPE_ITERATOR)
      && (type != OSL_SYMBOL_TYPE_ITERATOR)) {
    yyerror("illegal use of an iterator (update or reference) in a statement");
    return 0;
  }

  if ((symbol->type == OSL_SYMBOL_TYPE_PARAMETER)
      && (type != OSL_SYMBOL_TYPE_PARAMETER)) {
    yyerror("illegal use of a parameter (update or reference) in a statement");
    return 0;
  }

  symbol->type = type;

  if (symbol->type == OSL_SYMBOL_TYPE_ARRAY) {
    if (symbol->extent != NULL)
      osl_generic_free(symbol->extent);
    gen = osl_generic_shell(osl_relation_clone(access->elt),
                            osl_relation_interface());
    symbol->extent = gen;
  }
  return 1;
}



/**
 * clan_symbol_extent_from_osl_relation function:
 * During parsing, the array's access relation is stored in the "extent" field.
 * this function removes the rows of teh access relation. 
 * 
 * \param[in,out] access  Access relation stored in extent 
 * \return
 */
static void clan_symbol_extent_from_osl_relation(osl_relation_p access) {
  int i = 0;
  osl_relation_p extent = access;

  for (i=1; i <= extent->nb_input_dims; i++){
    osl_relation_remove_column(extent, extent->nb_output_dims+1);
  }
  for (i=0; i< extent->nb_output_dims-1; i++){
    osl_relation_remove_row(extent, extent->nb_rows-1);
  }
  osl_relation_set_attributes(extent,
                              access->nb_output_dims,
                              0,
                              access->nb_local_dims,
                              access->nb_parameters);
  return;
}

/**
 * clan_symbol_compact function:
 * this function compacts the dimensions of the symbol_extent (access relation) 
 * in the symbol table.
 * \param[in,out] symbol        The first symbol of the symbol table
 * \param[in]     nb_parameters Number of parameters
 * \return
 */
void clan_symbol_compact(osl_symbols_p symbol, int nb_parameters) {
  while (symbol) {
    if(symbol->type == OSL_SYMBOL_TYPE_ARRAY){
      clan_relation_compact(symbol->extent->data, nb_parameters);
      clan_symbol_extent_from_osl_relation(symbol->extent->data);
    }
    symbol = symbol->next;
  }
}

/**
 * clan_symbol_upate_array_dims function:
 * Array dimensios are discovered one by one during parsing and updated
 * respectively.
 * This function updates replaces the old access relation (extent) for
 * an array symbol, with the new one containing updated dimensions.
 *
 * \param[in,out] table The first symbol of the symbol table
 * \param[in]     acc   Access relation containing updated dimensions
 * \return
 */
void clan_symbol_upate_array_dims(osl_symbols_p table, osl_relation_p acc){
  int key = -1;
  int relation_type = -1;
  osl_symbols_p symbol = NULL;
  osl_generic_p gen = NULL;

  // Get the key (with some cheating with the relation type to be able to use
  // osl_relation_get_array_id), find the corresponding symbol and update.
  relation_type = acc->type;
  acc->type = OSL_TYPE_READ;
  key = osl_relation_get_array_id(acc);
  acc->type = relation_type;
  symbol = clan_symbol_lookup_by_key(table, key);
  if (symbol == NULL)
    CLAN_error("no symbol corresponding to the key");

  assert (symbol->type == OSL_SYMBOL_TYPE_ARRAY);

  if (symbol->extent != NULL){
    if(((osl_relation_p)symbol->extent->data)->nb_output_dims
                                    >= acc->nb_output_dims)
      return;
    else
      osl_generic_free(symbol->extent);
  }
  gen = osl_generic_shell(osl_relation_clone(acc),
                            osl_relation_interface());
  symbol->extent = gen;
}
