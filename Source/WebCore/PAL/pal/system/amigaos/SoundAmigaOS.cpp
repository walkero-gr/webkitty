#include "config.h"
#include "Sound.h"

#include <proto/intuition.h>

namespace PAL {

void systemBeep() { IIntuition->DisplayBeep(NULL); }

} // namespace PAL
