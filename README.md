# WAVE.wasm.js
Fast version of [WAVE.js](https://github.com/TakeshiOkamoto/WAVE.js). ( WAVE.jsの高速版。)
  
Use WebAssembly of HTML5 to write faster than 10x. improved reading speed by 30%.  
( HTML5のWebAssemblyを使用して書き込み速度を10倍以上に高速化。読み込み速度は改良して30%向上。)  
  
Succeeded in speeding is C language in WebAssembly. but, IE11 is not supported,  because it is the latest technology.  
( WebAssemblyでC言語を使用して高速化に成功しています。但し、最新技術なのでIE11は未対応です。 )  
  
It's as fast as OS native apps.  
( OSのネイティブアプリのように高速です。 )
  
## In My Blog  
To use WAVE.wasm.js with the "In My Blog" example from [WAVE.js](https://github.com/TakeshiOkamoto/WAVE.js), convert the code as follows.   
( WAVE.jsにある"In My Blog"の例をWAVE.wasm.jsで使用するには次のようにコードを変換して下さい。 )  
  
Audio combination. ( 音声の結合 )   
Audio extraction. ( 音声の抽出 )  
Audio delete. ( 音声の削除 )  
Add soundless. ( 無音の追加 )  
Convert to reverse playback. ( 逆再生に変換 )   
  
```rb
// data.L = tmpL;  
// data.R = tmpR;  
    
data.L = new Int32Array(tmpL);  
data.R = new Int32Array(tmpR);
```   
  
# Licence
MIT license  
