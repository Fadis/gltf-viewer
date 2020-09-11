* 3DモデルフォーマットglTFに詳しくなる本 サンプルプログラム

# ビルド方法

ソースコードをcheckoutしたディレクトリをSOURCEDIRとする


```shell
$ mkdir build
$ cd build
$ BUILDDIR=`pwd`
$ cmake ${SOURCEDIR}
$ make
$ pushd ${SOURCEDIR}/shaders
$ PATH=${PATH}:${BUILDDIR}/src/ ./compile.sh
$ popd
```

# 実行方法

```shell
$ cd ${BUILDDIR}
$ ./src/draw -i <glTFのファイルパス> -s ${SOURCEDIR}/shaders
```

