# -
本项目聚焦于实现先进的人脸识别功能，借助 LBPH 算法达成精准身份识别。以下是具体的功能亮点和技术细节： 核心功能 数据加载与增强：从数据库读取照片，解码为灰度图像并调整尺寸，同时运用随机旋转 -15 到 15 度等数据增强手段，丰富训练数据，提升模型泛化能力。 模型训练：利用加载的训练图像和对应标签对 LBPH 人脸识别器进行训练，确保模型能够准确学习不同人脸的特征。 人脸检测与识别：对输入图片进行人脸检测，通过高斯模糊和直方图均衡化预处理图像，提高检测精度。检测到人脸后，提取特征并与训练模型比对，输出识别结果和置信度。
