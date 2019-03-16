#include <math.h>

#define WASM_EXPORT __attribute__((visibility("default")))

// ---------------
//  型変換
// ---------------

int WAV_SetInt8(unsigned int x){
  if(128 == x) return 0;
 
  if(129 >= x){
    return x -128; 
  }else{
    return -(128 - x);
  }
}

int WAV_SetInt16(unsigned int x){
  if(32768 <= x){
    return  -(65536 - x); 
  }else{
    return x;
  }
}

int WAV_SetInt24(unsigned int x){
  if(8388608 <= x){
    return  -(16777216 - x); 
  }else{
     return x;
  }
}

int WAV_SetInt32(unsigned int x){
  if(2147483648 <= x){
    return  -(4294967296 - x); 
  }else{
     return x;
  }
}

unsigned int WAV_SetUint8(int x){
  if(0 == x) return 128;  
 
  if(1 >= x){
    return x + 128; 
  }else{
    return 128 + x;
  }
}

unsigned int WAV_SetUint16(int x){
  if(0 <= x && 32767 >= x){
    return x; 
  }else{
    return 65536 + x;
  }
} 

unsigned int WAV_SetUint24(int x){
  if(0 <= x && 8388607 >= x){
    return x; 
  }else{
    return 16777216 + x;
  }
} 

unsigned int WAV_SetUint32(int x){
  if(0 <= x && 2147483647 >= x){
    return x; 
  }else{
    return 4294967296 + x;
  }
} 

// ---------------
//  エキスポート
// ---------------

WASM_EXPORT
void read(void* p, int size_L, int size_R, int src_size, int bits, int channels) { 
  int index = 0, l=0, r=0 ;
  int* L = (int*)(p);   
  int* R = (int*)(p + size_L);   
  unsigned char* src = (unsigned char*)(p + size_L + size_R); 

  unsigned int value;

  if(bits == 8){
    // モノラル
    if(channels == 1){
      for(int i=0;i<src_size;i++){
        L[l++] = WAV_SetInt8(src[i]) ;                
      }   
                 
    // ステレオ  
    }else{
      for(int i=0;i<src_size;i++){
        if(i % 2 == 0){           
          L[l++] = WAV_SetInt8(src[i]) ;
        }else{
          R[r++] = WAV_SetInt8(src[i]) ;
        }
      }                   
    }
  }else if(bits == 16){
    if(channels == 1){
      for(int i=0;i<src_size/2;i++){   
        index = i * 2;   
        L[l++] = WAV_SetInt16((src[index+1] << 8) | src[index]) ;
      }              
    }else{
      for(int i=0;i<src_size/2;i++){
        index = i * 2; 
        if(i % 2 == 0){           
          L[l++] = WAV_SetInt16((src[index+1] << 8) | src[index]) ;
        }else{
          R[r++] = WAV_SetInt16((src[index+1] << 8) | src[index]) ;
        }
      }         
    }
  }else if(bits == 24){
    if(channels == 1){
      for(int i=0;i<src_size/3;i++){   
        index = i * 3;         
        L[l++] = WAV_SetInt24( src[index]          |
                              (src[index+1] << 8 ) |
                              (src[index+2] << 16)) ;                            
      }                     
    }else{
      for(int i=0;i<src_size/3;i++){
        index = i * 3; 
        if(i % 2 == 0){           
          L[l++] = WAV_SetInt24( src[index]          |
                                (src[index+1] << 8 ) |
                                (src[index+2] << 16)) ;  
        }else{
          R[r++] = WAV_SetInt24( src[index]          |
                                (src[index+1] << 8 ) |
                                (src[index+2] << 16)) ; 
        }
      }             
    }
  }else if(bits == 32){
    if(channels == 1){  
      for(int i=0;i<src_size/4;i++){
        index = i * 4;       
        L[l++] = WAV_SetInt32( src[index]          |
                              (src[index+1] << 8 ) |
                              (src[index+2] << 16) |
                              (src[index+3] << 24)); 
      }            
    }else{
      for(int i=0;i<src_size/4;i++){
        index = i * 4; 
        if(i % 2 == 0){           
          L[l++] = WAV_SetInt32( src[index]          |
                                (src[index+1] << 8 ) |
                                (src[index+2] << 16) |
                                (src[index+3] << 24)); 
        }else{
          R[r++] = WAV_SetInt32( src[index]          |
                                (src[index+1] << 8 ) |
                                (src[index+2] << 16) |
                                (src[index+3] << 24)); 
        }
      }            
    }
  }
}

WASM_EXPORT
void write(void* p, int size_L, int size_R, int bits, int channels) { 
  int index = 0;
  int* L = (int*)(p);   
  int* R = (int*)(p + size_L);   
  unsigned char* dst = (unsigned char*)(p + size_L + size_R + 44); 

  unsigned int value;

  if(bits == 8){
    for(int i=0;i<size_L/4;i++){
      // L
      dst[index++] =  WAV_SetUint8(L[i]);
      
      // R
      if(channels == 2){
        dst[index++] =  WAV_SetUint8(R[i]);
      }      
    }
  }else if(bits == 16){
    for(int i=0;i<size_L/4;i++){
      value = WAV_SetUint16(L[i]);
      dst[index++] =  value & 0x00FF;
      dst[index++] = (value & 0xFF00) >> 8;

      if(channels == 2){
        value = WAV_SetUint16(R[i]);
        dst[index++] =  value & 0x00FF;
        dst[index++] = (value & 0xFF00) >> 8;
      }      
    }
  }else if(bits == 24){
    for(int i=0;i<size_L/4;i++){
      value = WAV_SetUint24(L[i]);
      dst[index++] =  value & 0x0000FF;
      dst[index++] = (value & 0x00FF00) >> 8;
      dst[index++] = (value & 0xFF0000) >> 16;
      
      if(channels == 2){
        value = WAV_SetUint24(R[i]);
        dst[index++] =  value & 0x0000FF;
        dst[index++] = (value & 0x00FF00) >> 8;
        dst[index++] = (value & 0xFF0000) >> 16;
      }      
    }    
  }else if(bits == 32){
    for(int i=0;i<size_L/4;i++){
      value = WAV_SetUint32(L[i]);
      dst[index++] =  value & 0x000000FF;
      dst[index++] = (value & 0x0000FF00) >> 8;
      dst[index++] = (value & 0x00FF0000) >> 16;
      dst[index++] = (value & 0xFF000000) >> 24;
      
      if(channels == 2){
        value = WAV_SetUint32(R[i]);
        dst[index++] =  value & 0x000000FF;
        dst[index++] = (value & 0x0000FF00) >> 8;
        dst[index++] = (value & 0x00FF0000) >> 16;
        dst[index++] = (value & 0xFF000000) >> 24;
      }
    }
  }
}

WASM_EXPORT
int myMax(int *p, int size) {
  int max = p[0];
  for(int i=0;i<size;i++){
    if(max < p[i]){
      max = p[i];  
    }
  }
  return max;
}

WASM_EXPORT
int myMin(int *p, int size) {
  int min = p[0];
  for(int i=0;i<size;i++){
    if(min > p[i]){
      min = p[i];  
    }
  }
  return min;
}

// ---------------
//  未使用
// ---------------

WASM_EXPORT
void convert(void* p, int size, int bits) {
  int index =0; 
  
  float* src = (float*)(p);   
  int*   dst = (int*)(p + size);   
    
  if(bits == 8){
    for(int i=0;i<size/4;i++){
      if(src[index] >= 0){
        dst[index] = round(src[index] * 127);
      }else{
        dst[index] = round(src[index] * 128);
      }        
      index++;
    }    
  }else if(bits == 16){ 
    for(int i=0;i<size/4;i++){
      if(src[index] >= 0){
        dst[index] = round(src[index] * 32767);
      }else{
        dst[index] = round(src[index] * 32768);
      }        
      index++;
    }   
  }else if(bits == 24){ 
    for(int i=0;i<size/4;i++){
      if(src[index] >= 0){
        dst[index] = round(src[index] * 8388607); 
      }else{
        dst[index] = round(src[index] * 8388608);
      }        
      index++;
    }  
  }else if(bits == 32){ 
    for(int i=0;i<size/4;i++){
      if(src[index] >= 0){
        dst[index] = round(src[index] * 2147483647); 
      }else{
        dst[index] = round(src[index] * 2147483648);
      }        
      index++;
    }         
  } 
}
