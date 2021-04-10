"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => vim plug
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

call plug#begin('~/.vim/plugged')

" NerdTree
Plug 'preservim/nerdtree'

" HTML CSS shortcut
Plug 'mattn/emmet-vim'

" Syntastic pour les vérifications d'erreurs 
Plug 'vim-syntastic/syntastic'

" Js syntax highlight
Plug 'othree/yajs.vim'

" Jsx syntax highlight
Plug 'mxw/vim-jsx'

" Fuzzyfind file
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'junegunn/fzf.vim'

" Autopair for parenthesis
Plug 'jiangmiao/auto-pairs'

" Intellisense
Plug 'neoclide/coc.nvim', {'branch': 'release'}

" Custom bar + tab
Plug 'vim-airline/vim-airline'

" gruvbox
Plug 'morhetz/gruvbox'

" indent guides
Plug 'Yggdroot/indentLine'

" git fugitive
Plug 'tpope/vim-fugitive'

" Initialize plugin system
call plug#end()
