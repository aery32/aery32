## [API Documentations](http://devzone.aery32.com/aery32/apidoc)

This branch holds the doxygen genereated api documentations of Aery32 Framework

    git checkout master
    doxygen
    mv apidoc ../
    git checkout gh-pages
    cp -r ../apidoc/html/* apidoc
    git add .
    git commit -m "update apidocs"
    git push origin gh-pages
    git checkout master