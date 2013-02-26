{-# LANGUAGE ScopedTypeVariables #-}
{-# LANGUAGE ViewPatterns #-}
{-# LANGUAGE TupleSections #-}
module Main where
import Control.Monad
import Control.Applicative
import Control.Monad.Random
import Control.Monad.Trans.State.Lazy
import Control.Monad.Trans
import qualified Data.Set as S
import qualified Data.Map as M
import Control.Applicative
import Data.Maybe
import Data.List

import Data.Ord
import System.Environment

-- Worry about testing!

-- type Sample a b = StateT (Integer, Integer, [a]) (Random b)

-- need
accum :: (z -> a -> (b,z)) -> z -> [a] -> [b]
accum = undefined

-- but thats: mapM in lazy state.

-- test = foldr f undefined 0
--   where f x rest sum = sum : rest (x + sum)
-- test' l = evalState (mapM f l) 0
--   where f x = modify (x+) >> get

-- eval' (fmap (fromIntegral . (1+)) -> is) = zipWith (/) (tail is) is

-- evals ls = zip ls (eval' ls)

eval1 :: [(Int, Double)] -> [Double]
eval1 = map (\(x,p) -> (1-p) ^ x)

count :: [(Bool, Double, a)] -> [(Int, Double)]
count bs = case dropWhile (not . (\(b,_,_) -> b) . head . fst)
                $ zip (init $ tails bs) [0..] of
    (((_,y,_):bs'), c) : _ -> (c,y) : count bs'
-- main =
    otherwise -> []

numbers :: (Functor m, MonadRandom m) => Int -> m [(Bool, Double, S.Set Double)]
numbers c = evalStateT (mapM f =<< lift (getRandomRs (0.0, 1.0))) $ S.fromList ([1..fromIntegral c])
    where f (r :: Double)
              = do (y,s') <- gets (fromJust . S.maxView)
                   (if r < y
                       then (True,,) <$ put (S.insert r s')
                       else return (False,,)) <*> pure y <*> get

replaceBiggest :: Ord k => k -> S.Set k -> (S.Set k, Bool)
replaceBiggest k s@(S.maxView -> Just (y,s')) | k < y = (S.insert k s', True)
                                              | otherwise = (s, False)
replaceBiggest k s = (S.insert k s, True)



-- skips :: (Functor m, MonadRandom m) => Int -> m [Int]
-- skips c = S.fromList [1..fromIntegral c] where
--   f = do s <- getRandomR (0.0, 1.0)
--          (min 1 -> p, s') <- getS (fromJust . S.maxView)
--          let skip = floor $ log s / log (1-p)
--          tell $ return skip

-- -- Something like this:

-- sample :: [(Integer, Int)] -> [a] -> [a]
-- sample ((skip,what):rest) l = 
--     map h
--     modify (\(l, m) -> let (h:restl) = drop skip l in
--              (restl, M.insert what h m)
--   where
--     h (skip, what) = replicate

skips1 :: (Functor m, MonadRandom m) => Int -> Int -> m [(Integer, Int)]
skips1 n c =
    do s0 <- liftM M.fromList $
             mapM (\c -> (, c) <$> getRandomR (1,2))
             [1 .. c]
       evalStateT (sequence $ replicate n f) s0 where
  f = do (r :: Double) <- getRandomR (0.0, 1.0)
         ((min 1 -> p,pos), s) <- gets (fromJust . M.maxViewWithKey)
         (transform p r, pos) <$ do put =<< (M.insert <$> getRandomR (0.0, p) <*> pure pos <*> pure s)
  list = [1..fromIntegral c]

-- mapAccumL $ getRandomRs (0.0,1.0) where
--  ns = replicate c 0 ++ foldr f undefined c
--  f x rest corpus =

-- ln (U) / ln (1-p)
-- (1-p) ** x = U

transform p x = floor $ log x / log (1-p)


main = do
    args <- getArgs
    -- print args
    let k = 10
    rg <- getStdGen
    let (r :: [Double]) = randomRs (0,1.0) rg
--    putStr . (unlines . map snd . take k . sortBy (comparing fst)) .
--        zip r . lines =<< getContents
--    print . (maximumBy (comparing fst)) .
--        zip [0,0..] . lines =<< getContents

    print . length . map fst . zip r . lines =<< getContents

    -- l' <- flip zip ls <$> getRandomRs (0,1.0)
    -- sortBy (comparing fst)

-- sample :: a -> Sample a b -> Sample a b
-- sample a = do
--     r <- getRandom
--     (seen, size, corpus) <- get
--     if seen <= size
--         then put (seen+1, size+1, a:corpus)
--         else if r <= size/(seen-size)
--              then
--                put (seen+1, size,)

-- sample :: Int -> [a] -> [[a]]
-- sample n xs | null rest = corpus 0
--             | otherwise = foldr f (0, corpus) rest
--   where
--   corpus0 = take n xs
--   rest = drop n xs
--   f item (size, corpus) = undefined

-- main = do lines <- lines <$> getContents
--          print lines