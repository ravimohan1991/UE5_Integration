/////////////////////////////////////////////////////////////////
// TagWme class
//
// Author: Douglas Pearson, www.threepenny.net
// Date  : September 2004
//
// Represents an SML node which represents an argument for a command and has
// the tag <wme>. Wmes are the Working memory elements. 
//
/////////////////////////////////////////////////////////////////

#include "sml_TagWme.h"
#include "portability.h"

using namespace sml ;

TagWme::TagWme()
{
    this->SetTagNameFast(sml_Names::kTagWME) ;
}
