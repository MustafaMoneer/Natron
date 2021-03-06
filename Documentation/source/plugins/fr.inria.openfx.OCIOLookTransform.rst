.. _fr.inria.openfx.OCIOLookTransform:

OCIOLookTransform node
======================

|pluginIcon| 

*This documentation is for version 1.0 of OCIOLookTransform.*

Description
-----------

OpenColorIO LookTransform

A 'look' is a named color transform, intended to modify the look of an image in a 'creative' manner (as opposed to a colorspace definion which tends to be technically/mathematically defined).

Examples of looks may be a neutral grade, to be applied to film scans prior to VFX work, or a per-shot DI grade decided on by the director, to be applied just before the viewing transform.

OCIOLooks must be predefined in the OpenColorIO configuration before usage, and often reference per-shot/sequence LUTs/CCs.

See the 'Look Combination' parameter for further syntax details.

See opencolorio.org for look configuration customization examples.

Inputs
------

+----------+---------------+------------+
| Input    | Description   | Optional   |
+==========+===============+============+
| Source   |               | No         |
+----------+---------------+------------+
| Mask     |               | Yes        |
+----------+---------------+------------+

Controls
--------

.. tabularcolumns:: |>{\raggedright}p{0.2\columnwidth}|>{\raggedright}p{0.06\columnwidth}|>{\raggedright}p{0.07\columnwidth}|p{0.63\columnwidth}|

.. cssclass:: longtable

+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Parameter / script name                        | Type      | Default                     | Function                                                                                                                                                                             |
+================================================+===========+=============================+======================================================================================================================================================================================+
| OCIO Config File / ``ocioConfigFile``          | N/A       |                             | OpenColorIO configuration file                                                                                                                                                       |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Input Colorspace / ``ocioInputSpaceIndex``     | Choice    |                             | Input data is taken to be in this colorspace.                                                                                                                                        |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Single Look / ``singleLook``                   | Boolean   | On                          | When checked, only the selected Look is applied. When not checked, the Look Combination is applied.                                                                                  |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Look / ``lookChoice``                          | Choice    | Agfa Agfacolor Futura 100   | Look to apply (if "Single Look" is checked) or append to the Look Combination (when the "Append" button is pressed).                                                                 |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Append Look to Combination / ``append``        | Button    |                             | Append the selected Look to the Look Combination                                                                                                                                     |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Look Combination / ``lookCombination``         | String    |                             | | Specify the look(s) to apply.                                                                                                                                                      |
|                                                |           |                             | | This may be empty, the name of a single look, or a combination of looks using the 'look syntax'.                                                                                   |
|                                                |           |                             | | If it is empty, no look is applied.                                                                                                                                                |
|                                                |           |                             | | Look Syntax:                                                                                                                                                                       |
|                                                |           |                             | | Multiple looks are combined with commas: 'firstlook, secondlook'                                                                                                                   |
|                                                |           |                             | | Direction is specified with +/- prefixes: '+firstlook, -secondlook'                                                                                                                |
|                                                |           |                             | | Missing look 'fallbacks' specified with \|: 'firstlook, -secondlook \| -secondlook'                                                                                                |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Direction / ``direction``                      | Choice    | Forward                     | Transform direction.                                                                                                                                                                 |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Output Colorspace / ``ocioOutputSpaceIndex``   | Choice    |                             | Output data is taken to be in this colorspace.                                                                                                                                       |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| key1 / ``key1``                                | String    |                             | | OCIO Contexts allow you to apply specific LUTs or grades to different shots.                                                                                                       |
|                                                |           |                             | | Here you can specify the context name (key) and its corresponding value.                                                                                                           |
|                                                |           |                             | | Full details of how to set up contexts and add them to your config can be found in the OpenColorIO documentation:                                                                  |
|                                                |           |                             | | http://opencolorio.org/userguide/contexts.html                                                                                                                                     |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| value1 / ``value1``                            | String    |                             | | OCIO Contexts allow you to apply specific LUTs or grades to different shots.                                                                                                       |
|                                                |           |                             | | Here you can specify the context name (key) and its corresponding value.                                                                                                           |
|                                                |           |                             | | Full details of how to set up contexts and add them to your config can be found in the OpenColorIO documentation:                                                                  |
|                                                |           |                             | | http://opencolorio.org/userguide/contexts.html                                                                                                                                     |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| key2 / ``key2``                                | String    |                             | | OCIO Contexts allow you to apply specific LUTs or grades to different shots.                                                                                                       |
|                                                |           |                             | | Here you can specify the context name (key) and its corresponding value.                                                                                                           |
|                                                |           |                             | | Full details of how to set up contexts and add them to your config can be found in the OpenColorIO documentation:                                                                  |
|                                                |           |                             | | http://opencolorio.org/userguide/contexts.html                                                                                                                                     |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| value2 / ``value2``                            | String    |                             | | OCIO Contexts allow you to apply specific LUTs or grades to different shots.                                                                                                       |
|                                                |           |                             | | Here you can specify the context name (key) and its corresponding value.                                                                                                           |
|                                                |           |                             | | Full details of how to set up contexts and add them to your config can be found in the OpenColorIO documentation:                                                                  |
|                                                |           |                             | | http://opencolorio.org/userguide/contexts.html                                                                                                                                     |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| key3 / ``key3``                                | String    |                             | | OCIO Contexts allow you to apply specific LUTs or grades to different shots.                                                                                                       |
|                                                |           |                             | | Here you can specify the context name (key) and its corresponding value.                                                                                                           |
|                                                |           |                             | | Full details of how to set up contexts and add them to your config can be found in the OpenColorIO documentation:                                                                  |
|                                                |           |                             | | http://opencolorio.org/userguide/contexts.html                                                                                                                                     |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| value3 / ``value3``                            | String    |                             | | OCIO Contexts allow you to apply specific LUTs or grades to different shots.                                                                                                       |
|                                                |           |                             | | Here you can specify the context name (key) and its corresponding value.                                                                                                           |
|                                                |           |                             | | Full details of how to set up contexts and add them to your config can be found in the OpenColorIO documentation:                                                                  |
|                                                |           |                             | | http://opencolorio.org/userguide/contexts.html                                                                                                                                     |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| key4 / ``key4``                                | String    |                             | | OCIO Contexts allow you to apply specific LUTs or grades to different shots.                                                                                                       |
|                                                |           |                             | | Here you can specify the context name (key) and its corresponding value.                                                                                                           |
|                                                |           |                             | | Full details of how to set up contexts and add them to your config can be found in the OpenColorIO documentation:                                                                  |
|                                                |           |                             | | http://opencolorio.org/userguide/contexts.html                                                                                                                                     |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| value4 / ``value4``                            | String    |                             | | OCIO Contexts allow you to apply specific LUTs or grades to different shots.                                                                                                       |
|                                                |           |                             | | Here you can specify the context name (key) and its corresponding value.                                                                                                           |
|                                                |           |                             | | Full details of how to set up contexts and add them to your config can be found in the OpenColorIO documentation:                                                                  |
|                                                |           |                             | | http://opencolorio.org/userguide/contexts.html                                                                                                                                     |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| OCIO config help... / ``ocioHelpLooks``        | Button    |                             | Help about the OpenColorIO configuration.                                                                                                                                            |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Enable GPU Render / ``enableGPU``              | Boolean   | On                          | | Enable GPU-based OpenGL render.                                                                                                                                                    |
|                                                |           |                             | | If the checkbox is checked but is not enabled (i.e. it cannot be unchecked), GPU render can not be enabled or disabled from the plugin and is probably part of the host options.   |
|                                                |           |                             | | If the checkbox is not checked and is not enabled (i.e. it cannot be checked), GPU render is not available on this host.                                                           |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| (Un)premult / ``premult``                      | Boolean   | Off                         | Divide the image by the alpha channel before processing, and re-multiply it afterwards. Use if the input images are premultiplied.                                                   |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Invert Mask / ``maskInvert``                   | Boolean   | Off                         | When checked, the effect is fully applied where the mask is 0.                                                                                                                       |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Mix / ``mix``                                  | Double    | 1                           | Mix factor between the original and the transformed image.                                                                                                                           |
+------------------------------------------------+-----------+-----------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

.. |pluginIcon| image:: fr.inria.openfx.OCIOLookTransform.png
   :width: 10.0%
