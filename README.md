# **am-i-blind**

a ***revolutionary***, one of a kind, totally legit, **W**, `file / directory` finder tool

## **Motivation**

I absolutely love the default `find` tool <3

![me hating on the default find tool](https://i.imgur.com/sxWFJKJ.gif)

`wow.txt` not found? maybe I'm not using the correct **option** let me check the **help**

![really long help for a simple find tool](https://i.imgur.com/X1OPrzW.png)

***Ah yes!*** let me read a whole **fuqing essay** before I can find a simple **fuqing file**

## **How to Use**

```bash
Usage: am-i-blind [OPTIONS] <name> [path]

-r, --recursive      Search Recursively
-e, --exact          Match the name exactly
-f, --file           Only search for files
-d, --dir            Only search for directories
```

### **example**

```bash
am-i-blind wow.txt . -r # . = current directory
```

### **output**

```bash
./testing/wow.txt # wow.txt will be in red 
yes you are x1 times 
```

If no `file` or `directory` is found then the output is `no.` indicating you are not **blind**, *On the other hand* the output will contain all of the matches and the last line very ***respectfully*** telling you that you are blind *with number of matched results*

### **Note**

`files` will be highlighted **red**

`directories` will be highlighted **green**

