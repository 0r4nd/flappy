

#include "sample.h"



/******************************************************************************/

/**
 * set a sample object
 *
 * @param x     - x position
 * @return the sample_t structure
 */
sample_t sample_new(const char *filename)
{
  sample_t ret;
  sample_memset(&ret,0,sizeof(ret));
  return ret;
}

void sample_delete(sample_t *const self)
{
  assert(self);
}

