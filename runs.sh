
echo "yolov5模型 型号(s,m,l)： $1"

if [ ! -d "yolov5_deepsort_tensorrt_deploy/weights/" ]; then
  mkdir -p yolov5_deepsort_tensorrt_deploy/weights/
else
  rm -r yolov5_deepsort_tensorrt_deploy/weights/
  mkdir -p yolov5_deepsort_tensorrt_deploy/weights/
fi

cd yolov5_train/
python3 gen_wts.py -w ../model_origin/yolov5.pt -o ../yolov5_deepsort_tensorrt_deploy/weights/yolov5.wts

cd ../tensorrtx/yolov5-tensorrt/

if [ ! -d "/build" ]; then
  mkdir build
else
  rm -r build
  mkdir build
fi
cd build
cmake ..
make 

./yolov5 -s ../../../yolov5_deepsort_tensorrt_deploy/weights/yolov5.wts ../../../yolov5_deepsort_tensorrt_deploy/weights/yolov5.engine $1
echo "yolov5s.wts transfer sucess"

cd ../../../deepsort_train
python3 exportOnnx.py

echo "deepsort.onnx path"

cd ../tensorrtx/deepsort-tensorrt

if [ ! -d "/build" ]; then
  mkdir build
else
  rm -r build
  mkdir build
fi

cd build
cmake ..
make
./onnx2engine ../../../yolov5_deepsort_tensorrt_deploy/weights/deepsort.onnx ../../../yolov5_deepsort_tensorrt_deploy/weights/deepsort.engine

echo "deepsort.engine transfer sucess"

cd ../../../yolov5_deepsort_tensorrt_deploy

if [ ! -d "/build" ]; then
  mkdir build
else
  rm -r build
  mkdir build
fi

cd build
cmake ..
make
./yolosort