# CameLion
Perl storable/C struct back-and-forth translator

# Overview
CameLion can:
- Read and write perl-storable structures:
```
{
  field1 => value1,
  field2 => value2
}
```
- Provide access to any node, allowing to modificate it, remove or append any other node.
- Read and write perl-nfreezed binary data (nfreeze/thaw C-analogue)

# Basic

The main entity in the CameLion project is the CML_Node. You can:
## Create and free nodes:
```c
// Error checking is skipped due to readability. make sure to check every function

CML_Node * node;

CML_NodeCreate(CML_TYPE_STRING, &node);
...
CML_NodeFree(node);
```
Supported types are:
- `CML_TYPE_UNDEF`
- `CML_TYPE_INTEGER`
- `CML_TYPE_STRING`
- `CML_TYPE_ARRAY`
- `CML_TYPE_HASH`

## Name nodes:
```c
CML_NodeSetName(node, "mynode");

char nodename[] = {"Perfect"};
CML_NodeSetName(somenode, nodename);
```

## Assign values to nodes:
```c
CML_NodeSetInteger(node, 2);
CML_NodeSetString(node2, "String");
```
Notice, that you can't assign integer value to string-typed node and vice-versa.

## Access nodes values:
```c
node->data.integer // for INTEGER typed nodes
node->data.string  // for STRING typed nodes

node->nodes // For access to subnodes for ARRAY and HASH typed nodes
// Notice that perl-nfreezed data does not support named elements in ARRAYS, therefore the names for ARRAY elements will be ignored while saving to nfreezed data.
```

## Copy node:
```c
// Copy node with all subnodes:
CML_NodeCopy(src, &dst, CML_TRUE);

// Copy only name & value, without subnodes:
CML_NodeCopy(src, &dst, CML_FALSE);
```

## Print node as perl-storable:
```c
char * string;
CML_StorableToString(node, &string);

printf("%s\n", string);
free(string);
```
```
{
    vladislav => 'oh baby',
    dont => 'hurt me',
    no => [
        'more',
        'less',
        {
            wut => 'lolwut?',
        },
    ],
},
```
## Append/Insert/Remove nodes:
```c
CML_NodeAppend(dst, child);
CML_NodeInsert(dst, child, 2);
CML_NodeRemove(dst, 1);
```

## Find for subnodes:
```
{
  field1 => value1,
  field2 => {
    carrot => {
      vegetable => 'true',
      }
    }
  }
}
```
```c
CML_Node * root;
CML_Node * child;
CML_NodeFindString(root, "field2.carrot.vegetable", &child);

// Now you have access to 'vegetable' subnode via child variable.
```
You may use type-specified functions like `CML_NodeFindInteger` or generic function `CML_NodeFind` which takes a type as an argument.

# Perl interaction
## Storable <-> Node translation

You can load perl-storable into node via these functions:
```c
CML_Error CML_StorableFromFile  (char * filename, CML_Node ** result);
CML_Error CML_StorableFromString(char * storable, CML_Node ** result);
```
```c
char teststorable[] = {
    "{\n"
    "   branch1 => {\n"
    "       value1 => 1,\n"
    "       value2 => \"Deep into\",\n"
    "   },\n"
    "   branch2 => [\n"
    "       'Darkness peering',"
    "       'Long I \\'stood\\' there',\n"
    "       \"Wondering, fearing\","
    "   ],\n"
    "   branch3 => {\n"
    "       value3 => Dreaming,\n"
    "       value4 => Dreams,\n"
    "       value5 => \"No mortal ever dared to dream before\",\n"
    "   },\n"
    "}"
}

int main (void)
{
  CML_Node * poe;
  CML_StorableFromString(teststorable, &poe);
  <..>
```


And unload it back with these:
```c
CML_Error CML_StorableToFile  (CML_Node * node, char *  filename);
CML_Error CML_StorableToString(CML_Node * node, char ** storable);
```
```c
  <..>
  char * string;
  CML_StorableToString(poe, &string);
  printf("%s\n", string);
  free(string);
  CML_NodeFree(poe);
}
```

## Nfreezed data <-> Node translation
```c
CML_Error CML_ThawBytes(CML_Bytes * bytes, CML_Node ** result);
CML_Error CML_ThawFile (char * filename,   CML_Node ** result);
/// Notice that CML_ThawXXX functions allocate result variable

CML_Error CML_NfreezeNode    (CML_Node * node, CML_Bytes ** result);
CML_Error CML_NfreezeStorable(char * storable, CML_Bytes ** result);
// Notice that CML_NfreezeXXX functions allocate result variable on their own
```

If you want to save CML_Bytes to file you can use `CML_DataToFile` function:
```c
CML_DataToFile(bytes->data, bytes->size, "./myfilename.bin");
```
Also you can use this function to write any data including strings:
```c
char mystring[] = {"something"};
CML_DataToFile(mystring, strlen(mystring), "./stringy.txt");
```

# Using as settings manager

You can also use this library for settings management for c-projects. Just put your settings to perl-storable and place that file somewhere. The library will do the rest:
```perl
{
  messages => {
    info    => disabled,
    warnings => disabled,
    errors   => enabled
  }

  timing => {
    lowperiod    => 20, # Seconds
    mediumperiod => 10, # Seconds
    highperiod   =>  2  # Seconds
  }
}
```
```c
// Loading
CML_Node * settings;
CML_StorableFromFile("./settings.perl", &settings);
...

// Using
CML_Node * lowperiod;
CML_NodeFindInteger(settings, "timing.lowperiod", &lowperiod);
TimerSetPeriod(lowperiod->data.integer);
```

# Error checking

CameLion functions may return number of error codes (see `CML_X_ERROR_CODES`). To provide user-readable error description one may use `CML_HelpError` function:

```c
if (result != CML_ERROR_SUCCESS)
{
    printf("Error is %d: %s\n", result, CML_HelpError(result));
    exit(1);
}
```
```
Memory allocation failed
```

# Header file generation

If you want to use libcamelion.h in non-C project you may need to preprocess it to get rid of macros.

```sh
mv ./libcamelion.h ./_libcamelion.h
gcc -E ./_libcamelion.h -o ./libcamelion.h
indent -gnu -l200 -i4 -nut ./libcamelion.h
```
NOTE: using `indent` might need installation of this utility.
