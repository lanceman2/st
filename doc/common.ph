<?php

function stFileIndex()
{
  $script = basename($_SERVER["SCRIPT_FILENAME"]);
  echo "<div class=nav>\n";
  foreach(glob("*.phtml") as $gf)
  {
    $f = str_replace('.phtml','', $gf);
    if(basename($gf) !== $script)
      echo "<a style=\"font-size: 80%;\" href=\"".$f.".html\">".$f."</a>\n";
    else
      echo "<span style=\"font-size: 80%;\">".$f."</span>\n";
  }
  echo "</div>\n";
}

function stHeader($title)
{
  echo <<<END
<html lang=en>
  <head>
    <meta charset="UTF-8"> 
    <link rel="stylesheet" href="st.css">
    <title>$title</title>
    <!-- EDIT this MathJax file path -->
    <script id=localjaxscript
       src="../../../public_html/MathJax/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
    </script>
    <script>
      if('undefined' === typeof MathJax)
      {
        /* If we do not have
           ../../../public_html/MathJax/MathJax.js
           we get it from the following URL.
         */
         mathJaxSrc =
           "https://cdn.mathjax.org/mathjax/latest/MathJax.js" +
           "?config=TeX-AMS-MML_HTMLorMML"
         document.write(
            "<script src=\"" + mathJaxSrc +
            "\"> <" + "/" + "script" + ">"
         );
       }
       else
         mathJaxSrc = document.getElementById('localjaxscript').src
    </script>
    <script type="text/x-mathjax-config">
        MathJax.Hub.Config({ TeX: { equationNumbers: {autoNumber: "all"} } });
    </script>
  </head>
  <body>

    <script>
      document.write(
          "<h5>Using MathJax from " +
          mathJaxSrc +
          "<\h5>");
    </script>
END;

  stFileIndex();
}

function stFooter()
{

  stFileIndex();

  echo <<<END
  </body>
</html>
END;
}
?>
