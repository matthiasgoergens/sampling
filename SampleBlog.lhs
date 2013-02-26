> {-# LANGUAGE Rank2Types #-}
> module Main where
> import Control.Monad
> import Control.Applicative
> import Control.Monad.Random
> import Control.Monad.Trans.State.Lazy
> import Control.Monad.Trans
> import qualified Data.Set as S
> import qualified Data.Map as M
> import Control.Applicative
> import Data.Maybe
> import Data.List
> import Data.Function

import Data.Ord
import System.Environment


Producing a sample of k elements out of a bigger population of n
elements is a problem often encountered.  Any sampling algorithm can
be modelled as a function of type Sampler:

> type Sampler m a = MonadRandom m => Int -> [a] -> m [a]

Where the first Int parameter gives the size of the sample to be
returned.  It also has some extra structure not captured by the type:
We require any sampler to have a probability distribution equal to
@sampler'@:

> sampler' k l = take k . sortBy (compare `on` fst)
>                <$> (flip zip l <$> randomRs (0,1))

(TODO: Find a way of describing that doesn't give everything away.)

An online sampling algorithm has more structure:

> runOnline :: (Functor m, MonadRandom m)
>           => (Int -> a -> z -> m z)
>           -> (z -> [a])
>           -> z
>           -> Sampler m a
> runOnline next end start k l
>     = fmap end $ foldl next' (return start) l
>     where next' z' a = next k a =<< z'

I.e. it treats the list as a stream and looks as its elements one by
on.  An online sampler @runOnline next end start@ is characterized by
its kernel / update-function @next@.

(TODO: The choice of how to pass parameter k is awkward.)

Reservoir methods for online sampling are even more constrained: Their
intermediate state is basically restricted to being a sample found so
far.

> runReservoir next k l = runOnline next (either snd id)
>                         (Left (k,[]))




> main = return ()