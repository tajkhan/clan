
   /*+------- <| --------------------------------------------------------**
    **         A                     Clan                                **
    **---     /.\   -----------------------------------------------------**
    **   <|  [""M#                 symbol.h                              **
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


#ifndef CLAN_SYMBOL_H
# define CLAN_SYMBOL_H

#include <osl/extensions/symbols.h>

# if defined(__cplusplus)
extern "C"
  {
# endif

struct osl_strings;
struct osl_generic;
struct osl_relation_list;

/*+****************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/


/*+****************************************************************************
 *                    Memory allocation/deallocation function                 *
 ******************************************************************************/


/*+****************************************************************************
 *                            Processing functions                            *
 ******************************************************************************/
osl_symbols_p       clan_symbol_lookup(osl_symbols_p, char*);
osl_symbols_p       clan_symbol_lookup_by_key(osl_symbols_p, int);
osl_symbols_p       clan_symbol_add(osl_symbols_p*, char*, int);
int                 clan_symbol_get_rank(osl_symbols_p);
int                 clan_symbol_get_type(osl_symbols_p, char*);
struct osl_strings* clan_symbol_array_to_strings(osl_symbols_p*, int);
int                 clan_symbol_nb_of_type(osl_symbols_p, int);
struct osl_generic* clan_symbol_to_strings(osl_symbols_p, int);
osl_symbols_p       clan_symbol_clone_one(osl_symbols_p);
struct osl_generic* clan_symbol_to_arrays(osl_symbols_p);
int                 clan_symbol_new_iterator(osl_symbols_p*, osl_symbols_p*,
                                             char*, int); 
int                 clan_symbol_update_type(osl_symbols_p,
                                            struct osl_relation_list*, int);
char*               clan_symbol_get_identifier(osl_symbols_p);
int                 clan_symbol_get_rank(osl_symbols_p);
void                clan_symbol_set_rank(osl_symbols_p, int);
void                clan_symbol_compact(osl_symbols_p, int);
void                clan_symbol_upate_array_dims(osl_symbols_p, osl_relation_p);

# if defined(__cplusplus)
  }
# endif
#endif /* define CLAN_SYMBOL_H */
