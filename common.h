#pragma once

#define BLOCKW 7 
#define GSBW 10
#define GSBH 5
#define SSW (BLOCKW*GSBW+3+2) // 3+7*7+2
#define SSH (BLOCKW+2)  // 7+2
#define GSW (SSW)
#define GSH (BLOCKW*GSBH+2) // 5*7+2
#define SH  (SSH+GSH)
#define SW  (SSW)


