// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TO_STREAM_HPP
#define TAOCPP_JSON_INCLUDE_TO_STREAM_HPP

#include <cstddef>
#include <ostream>

#include "data.hpp"
#include "events/from_value.hpp"
#include "events/to_pretty_stream.hpp"
#include "events/to_stream.hpp"
#include "events/transformer.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class... Transformers >
      void to_stream( std::ostream& os, const data& v )
      {
         events::transformer< events::to_stream, Transformers... > consumer( os );
         events::from_value( consumer, v );
      }

      template< template< typename... > class... Transformers >
      void to_stream( std::ostream& os, const data& v, const std::size_t indent )
      {
         events::transformer< events::to_pretty_stream, Transformers... > consumer( os, indent );
         events::from_value( consumer, v );
      }

   }  // namespace json

}  // namespace tao

#endif
