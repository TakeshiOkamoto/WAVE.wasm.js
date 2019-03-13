#define WASM_EXPORT __attribute__((visibility("default")))

unsigned char dst[32768];
long L[4096];
long R[4096];

// ------------------------------------
//  アドレス
// ------------------------------------
WASM_EXPORT
long* getAddress_L() {
  return &L[0];
}

WASM_EXPORT
long* getAddress_R() {
  return &R[0];
}

WASM_EXPORT
unsigned char* getAddress_Dst() {
  return &dst[0];
}

// ------------------------------------
//  型変換
// ------------------------------------
unsigned long WAV_SetUint8(long x){
  if(0 == x) return 128;  
 
  if(1 >= x){
    return x + 128; 
  }else{
    return 128 + x;
  }
}

unsigned long WAV_SetUint16(long x){
  if(0 <= x && 32767 >= x){
    return x; 
  }else{
    return 65536 + x;
  }
} 

unsigned long WAV_SetUint24(long x){
  if(0 <= x && 8388607 >= x){
    return x; 
  }else{
    return 16777216 + x;
  }
} 

unsigned long WAV_SetUint32(long x){
  if(0 <= x && 2147483647 >= x){
    return x; 
  }else{
    return 4294967296 + x;
  }
} 

// ------------------------------------
//  実行
// ------------------------------------
WASM_EXPORT
void run(int size,int bits, int channels) { 
  int index = 0, count = 0;
  
  unsigned long value;
  if(bits == 8){
    while(1){
      // L
      dst[index++] = WAV_SetUint8(L[count]);      
      // R
      if(channels == 2){
        dst[index++] = WAV_SetUint8(R[count]);
      }
      
      count++;
      if(count == size) break;
    }       
  }else if(bits == 16){
    while(1){
      // L
      value = WAV_SetUint16(L[count]);
      dst[index++] =  value & 0x00FF;
      dst[index++] = (value & 0xFF00) >> 8;

      // R
      if(channels == 2){
        value = WAV_SetUint16(R[count]);
        dst[index++] =  value & 0x00FF;
        dst[index++] = (value & 0xFF00) >> 8;
      }
      
      count++;
      if(count == size) break;
    }     
  }else if(bits == 24){
    while(1){
      // L
      value = WAV_SetUint24(L[count]);
      dst[index++] =  value & 0x0000FF;
      dst[index++] = (value & 0x00FF00) >> 8;
      dst[index++] = (value & 0xFF0000) >> 16;
      
      // R
      if(channels == 2){
        value = WAV_SetUint24(R[count]);
        dst[index++] =  value & 0x0000FF;
        dst[index++] = (value & 0x00FF00) >> 8;
        dst[index++] = (value & 0xFF0000) >> 16;
      }
      
      count++;
      if(count == size) break;
    }     
  }else if(bits == 32){
    while(1){
      // L
      value = WAV_SetUint32(L[count]);
      dst[index++] = value  & 0x000000FF;
      dst[index++] = (value & 0x0000FF00) >> 8;
      dst[index++] = (value & 0x00FF0000) >> 16;
      dst[index++] = (value & 0xFF000000) >> 24;
       
      // R
      if(channels == 2){
        value = WAV_SetUint32(R[count]);
        dst[index++] = value  & 0x000000FF;
        dst[index++] = (value & 0x0000FF00) >> 8;
        dst[index++] = (value & 0x00FF0000) >> 16;
        dst[index++] = (value & 0xFF000000) >> 24;
      }
      
      count++;
      if(count == size) break;
    }   
  }
}
