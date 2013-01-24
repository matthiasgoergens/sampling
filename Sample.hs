import Control.Monad
import Control.Applicative


main = do lines <- lines <$> getContents
          print lines