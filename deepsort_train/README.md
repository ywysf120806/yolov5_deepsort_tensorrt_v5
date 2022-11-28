# deepsort 训练流程

## 第一步：数据准备(两种方式)
1.1 	直接准备的数据格式
>     datas------ 
>		 train
>	        dog-----
> 	             001jpg 
>		         002jpg
>	     	person-----
>                001jpg
>                002jpg
>	     validation
>	        dog-----
>                001jpg
>                002jpg
>	     	person---
>                001jpg
>                002jpg


1.2 通过xml标注的数据，做裁剪，进而数据转换成以上数据形式
> 1.2.1  xml 标注数据形式  

>   jpg_data---------  

>       000.jpg 
>       001.jpg 

>   xml_data---------   

>       000.xml 
>       001.xml 

运行脚本 python deep_sort/xml_crop_data.py jpg_data xml_data datas.  
   （注： jpg_data：jpg图片文件路径， xml_data：xml存放路径； datas：存放的数据路径）
> 1.2.2  生成的数据集形式  

	datas------  
	    dog---------
	        001jpg
		    002jpg
	    person-------
	        001jpg
		    002jpg



## 第二步： 模型训练  

>  更改 deepsort/deep/model.py 将其中的num_classes=751 改为项目中需要追踪的种类数. 

> 修改配置文件 configs/deep_sort.yaml 各参数
            （参数解析：
>>         RID_CKPT为deep模型存放的路径
>> 	     MAX_DIST：最大余弦距离，用于级联匹配；
>>         MIN_CONFIENCE:置信度阈值；
>>         NMS_MAX_OVERLAP：非极大值抑制阈值；
>>         MAX_IOU_DISTANCE：最大IOU阈值；
>>         MAX_AGE:最大寿命，即经过MAX_AGE帧没有追踪到该物体，删除该轨迹；
>>         N_INIT：最高击中次数，击中该次数，目标物由不确定态转为确定态；
>>         NN_BUDGET：最大保存特征帧数）

> 	运行脚本 python3 deep_sort/deep/train.py --data-dir datas_deep --gpu-id 1 --lr 0.001 --batch-size 64 --epochs 150


