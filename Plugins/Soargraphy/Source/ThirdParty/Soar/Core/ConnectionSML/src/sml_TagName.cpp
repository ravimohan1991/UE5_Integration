/////////////////////////////////////////////////////////////////
// TagName class
//
// Author: Douglas Pearson, www.threepenny.net
// Date  : October 2004
//
// Represents an SML node which represents a named object and
// has the tag <name>
//
/////////////////////////////////////////////////////////////////

#include "sml_TagName.h"
#include "portability.h"

using namespace sml ;

TagName::TagName()
{
    this->SetTagNameFast(sml_Names::kTagName) ;
}
