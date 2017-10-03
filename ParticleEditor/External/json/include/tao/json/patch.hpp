// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_PATCH_HPP
#define TAOCPP_JSON_INCLUDE_PATCH_HPP

#include <stdexcept>
#include <utility>

#include "data.hpp"
#include "pointer.hpp"

namespace tao
{
   namespace json
   {
      inline void patch_inplace( data& v, const data& patch )
      {
         for( const auto& entry : patch.get_array() ) {
            const auto& op = entry.at( "op" ).get_string();
            const auto& path = entry.at( "path" ).get_string();
            const pointer path_pointer( path );
            if( op == "test" ) {
               if( v.at( path_pointer ) != entry.at( "value" ) ) {
                  throw std::runtime_error( "test failed for: " + path );
               }
            }
            else if( op == "remove" ) {
               v.erase( path_pointer );
            }
            else if( op == "add" ) {
               v.insert( path_pointer, entry.at( "value" ) );
            }
            else if( op == "replace" ) {
               v.at( path_pointer ) = entry.at( "value" );
            }
            else if( op == "move" ) {
               const pointer from( entry.at( "from" ).get_string() );
               auto t = std::move( v.at( from ) );
               v.erase( from );
               v.insert( path_pointer, std::move( t ) );
            }
            else if( op == "copy" ) {
               const pointer from( entry.at( "from" ).get_string() );
               v.insert( path_pointer, v.at( from ) );
            }
            else {
               throw std::runtime_error( "unknown patch operation: '" + op + '\'' );
            }
         }
      }

      inline void patch_inplace( data& v, data&& patch )
      {
         for( const auto& entry : patch.get_array() ) {
            const auto& op = entry.at( "op" ).get_string();
            const auto& path = entry.at( "path" ).get_string();
            const pointer path_pointer( path );
            if( op == "test" ) {
               if( v.at( path_pointer ) != entry.at( "value" ) ) {
                  throw std::runtime_error( "test failed for: " + path );
               }
            }
            else if( op == "remove" ) {
               v.erase( path_pointer );
            }
            else if( op == "add" ) {
               v.insert( path_pointer, std::move( entry.at( "value" ) ) );
            }
            else if( op == "replace" ) {
               v.at( path_pointer ) = std::move( entry.at( "value" ) );
            }
            else if( op == "move" ) {
               const pointer from( entry.at( "from" ).get_string() );
               auto t = std::move( v.at( from ) );
               v.erase( from );
               v.insert( path_pointer, std::move( t ) );
            }
            else if( op == "copy" ) {
               const pointer from( entry.at( "from" ).get_string() );
               v.insert( path_pointer, v.at( from ) );
            }
            else {
               throw std::runtime_error( "unknown patch operation: '" + op + '\'' );
            }
         }
      }

      data patch( data v, const data& patch )
      {
         patch_inplace( v, patch );
         return v;
      }

      data patch( data v, data&& patch )
      {
         patch_inplace( v, std::move( patch ) );
         return v;
      }

   }  // namespace json

}  // namespace tao

#endif
