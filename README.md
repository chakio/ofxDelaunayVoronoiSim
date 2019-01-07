[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
ofxDelaunayVoronoiSim
====
![result](https://github.com/chakio/ofxDelaunayVoronoiSim/blob/master/media/voronoi720.gif) 
## Description
ドロネー三角分割とボロノイ図の描画  
円の中を動きまわる複数の赤色の点の位置に基づき、平面に対しドロネー三角分割（<font color="Red">赤線</font>）とボロノイ図（<font color="Blue">青線</font>）を描画しています   
<font color="Green">緑の点</font>は円とボロノイ図の交点です 
[ドロネー三角分割](https://ja.wikipedia.org/wiki/%E3%83%89%E3%83%AD%E3%83%8D%E3%83%BC%E5%9B%B3)：散らばった点をポリゴン化された図形（交差しないように各点を繋いだ図形）    
[ボロノイ図](https://ja.wikipedia.org/wiki/%E3%83%9C%E3%83%AD%E3%83%8E%E3%82%A4%E5%9B%B3)：散らばった点の位置に基づき領域分けされた図形    
ドロネー三角分割とボロノイ図は双対の関係にあるため同時に描画することが可能です。  
ボロノイ図が非常に非線形的な動きをするため、なんとなく生き物を見ているような感覚に陥ります。  

## Environment
* Ubuntu16.04
* openFrameworks v0.10.0
* visualstudio code

## Lisence
[MIT](https://github.com/chakio/ofxDelaunayVoronoiSim/blob/master/LICENSE)

## Author
[chakio](https://github.com/chakio)
