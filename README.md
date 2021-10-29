# - 測溫散熱系統

<img src="https://github.com/g91358677462/-/blob/main/assets/%E6%B8%AC%E6%BA%AB%E6%95%A3%E7%86%B1%E7%B3%BB%E7%B5%B1%E4%BD%9C%E5%93%81%E5%9C%96.png" width="50%" height="50%">

使用元件: 8051晶片(微控制器)、DS18B20(感溫元件)、七段顯示器、風扇(12V, 降溫元件)、7805-CT(變壓器)、RAS-0515(繼電器)。 

功能說明: 
1. 監測溫度: 對DS18B20進行序列埠的操作，將DS18B20回傳給8051的溫度資料顯示在7段顯示器上。 
2. 設定控溫閥值: DS18B20監測到目標溫度超過設定溫度的門檻值，則8051會送訊號開啟Relay，驅動12V風扇轉動來做降溫。 

電路說明: 外接電源為12V，用來驅動12V風扇，但8051供電為5V，所以用個"7805-CT 線性電壓穩壓器"來做降低電壓的動作。

# - 流程圖說明
<img src="https://github.com/g91358677462/-/blob/main/assets/%E6%B5%81%E7%A8%8B%E5%9C%96.png" width="50%" height="50%">

# - 電路說明
<img src="https://github.com/g91358677462/-/blob/main/assets/%E9%9B%BB%E8%B7%AF%E8%A8%AD%E8%A8%88%E6%A6%82%E5%BF%B5%E6%96%B9%E5%A1%8A%E5%9C%96.PNG" width="50%" height="50%">
其外接電源為12V，用來驅動12V風扇，但8051供電為5V，所以用個"7805-CT 線性電壓穩壓器"來做降低電壓的動作。
