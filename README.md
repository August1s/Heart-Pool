# Heart-Pool
Fluid simulation based on smoothed particle hydrodynamics

### 环境
- 编程环境 vs2019
- glfw\-3.3.2
- glew\-2.1.0
- Eigen3

### 简要说明

1. 计算上使用了基于SPH的流体模拟，使用欧拉方法解递推方程。使用R键可以在运行过程中重新开始流体模拟，使用Esc键关闭窗口。
2. 代码中实现了分别实现了对心形线与圆的边界判定。为了整理过程更加平滑，边界没有对反弹速度进行damping处理；为了达到一种类似“沸腾”的效果，在位移控制方面，在原位置的基础上反向增加了一个增量，可以使粒子拥有跳动效果。
3. 粒子会有一定几率出现“坍缩”现象，这是因为欧拉方法有时并不收敛。

### 参考链接
[Particle-Based Fluid Simulation with SPH](https://lucasschuermann.com/writing/particle-based-fluid-simulation)


