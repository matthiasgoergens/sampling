import Control.Monad
import Control.Applicative
import Control.Random
import Control.Monad.Random

type Sample a b = StateT (Integer, Integer, [a]) (Random b)

sample :: a -> Sample a b -> Sample a b
sample a = do
    r <- getRandom
    (seen, size, corpus) <- get
    if seen <= size
        then put (seen+1, size+1, a:corpus)
        else if r <= size/(seen-size)
             then 
               put (seen+1, size,)

sample :: Int -> [a] -> [a]
sample n xs | null rest = corpus 0
            | otherwise = foldr f (0, corpus) rest
  where
  corpus0 = take n xs
  rest = drop n xs
  f item (size, corpus) = undefined

main = do lines <- lines <$> getContents
          print lines