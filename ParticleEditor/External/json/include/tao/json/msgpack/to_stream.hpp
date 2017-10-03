// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_MSGPACK_TO_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_MSGPACK_TO_STREAM_HPP

#include <ostream>

#include "../data.hpp"

#include "../events/from_value.hpp"
#include "../events/transformer.hpp"

#include "../events/msgpack/to_stream.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         template< template< typename... > class... Transformers >
         void to_stream( std::ostream& os, const data& v )
         {
            events::transformer< events::msgpack::to_stream, Transformers... > consumer( os );
            events::from_value( consumer, v );
         }

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
