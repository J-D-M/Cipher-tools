import System.Environment
import Data.Char

atbash :: String -> String
atbash = map atbash'
  where
    atbash' :: Char -> Char
    atbash' a = toEnum $end - (ord a - shift)
      where end   = ord (if isUpper a then 'Z' else 'z')
            shift = ord (if isUpper a then 'A' else 'a')

main =
  getArgs >>= putStrLn . atbash . unwords
