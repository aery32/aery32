## [API Documentations](http://devzone.aery32.com/aery32/apidoc)

This branch holds the doxygen genereated api documentations of Aery32 Framework

    git checkout master
    cd aery32/
    doxygen
    mv doxies ../../
    git checkout gh-pages
    git rm -r apidoc/
    cp -r ../doxies/html/ apidoc
    git add apidoc/
    git commit -m "update apidocs"
    git push origin gh-pages
    git checkout master