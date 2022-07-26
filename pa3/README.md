To fix the following error:

```bash
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(4.2.0) ../modules/imgproc/src/color.cpp:182: error: (-215:Assertion failed) !_src.empty() in function 'cvtColor'
```

You need to replace obj_path in `main`
