#include "stdafx.h"
#include "ERTREObject.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

ERTREObject::ERTREObject()
{
    _uid = std::hash<std::string>()(boost::lexical_cast<std::string>(boost::uuids::random_generator()()));
}

