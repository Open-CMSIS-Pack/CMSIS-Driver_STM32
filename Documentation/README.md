# CMSIS-Driver_STM32 Pack Documentation

The documentation source is maintained in `Documentation/Doxygen/` folder as a mixture of markdown and doxygen formats. Component source files, templates and examples may also contribute information that gets integrated into the final documentation.

Generating the HTML-formatted documentation from the source is done with `gen_doc.sh` script:

```sh
$ ./Documentation/Doxygen/gen_doc.sh
```

The script expects specific version of [doxygen](https://www.doxygen.nl/) to be installed locally. After successful execution the resulting documentation package is then available in `./Documentation/html/`.