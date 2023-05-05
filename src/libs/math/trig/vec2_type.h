
#ifndef VEC2_TYPE_H_
#define VEC2_TYPE_H_


// Standard library.
#include <stdint.h> // Pour les int32_t, uint32_t ... .
#include <math.h>


/**
 * Tuple of 2 floats
 *
 * Each field can be addressed using several aliases:
 *  - First component:  <b>x</b> or <b>s</b>
 *  - Second component: <b>y</b> or <b>t</b>
 */
typedef union
{
  float data[2]; /**< All components at once */
  struct {
    float x; /**< Alias for first component  */
    float y; /**< Alias for second component */
  };
  struct {
    float s; /**< Alias for first component  */
    float t; /**< Alias for second component */
  };
  struct {
    float min; /**< Alias for first component  */
    float max; /**< Alias for second component */
  };
  struct {
    float start; /**< Alias for first component  */
    float end;   /**< Alias for second component */
  };


  /**< Integers */
  int32_t idata[2]; /**< All components at once */
  struct {
    int32_t ix; /**< Alias for first component  */
    int32_t iy; /**< Alias for second component */
  };
  struct {
    int32_t is; /**< Alias for first component  */
    int32_t it; /**< Alias for second component */
  };

} vec2;



#endif

