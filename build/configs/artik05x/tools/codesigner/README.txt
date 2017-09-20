CONTENTS

This tool is used for bl2 and os image signing of ARTIK055s models.
But, Please caution about below contents.

1. If you don't build a bl2 newly, you don't need a signing process.
   We provide a bl2 image as signed condition because it is pre-build image.
2. The codesigner should be used for OS image after every build time.
3. If you are B2B customer and you want to mass production, we recommand that
   you will use KMS system better than code signer.

USAGE
 : ./artik05x_codesigner -sign [input file]
