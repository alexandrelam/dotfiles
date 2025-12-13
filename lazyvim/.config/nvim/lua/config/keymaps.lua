-- Keymaps are automatically loaded on the VeryLazy event
-- Default keymaps that are always set: https://github.com/LazyVim/LazyVim/blob/main/lua/lazyvim/config/keymaps.lua
-- Add any additional keymaps here

vim.keymap.set({ "n", "x" }, "<leader>cp", function()
	local file_path = vim.fn.expand("%:.")
	local mode = vim.api.nvim_get_mode().mode
	local text = file_path

	-- Check if in visual mode (v, V, or Ctrl-V)
	if mode:match("[vV\22]") then
		local start_line = vim.fn.line("v")
		local end_line = vim.fn.line(".")

		-- Ensure start is always smaller than end
		if start_line > end_line then
			start_line, end_line = end_line, start_line
		end

		text = text .. ":" .. start_line
		if start_line ~= end_line then
			text = text .. "-" .. end_line
		end

		-- Exit visual mode after copying
		vim.api.nvim_feedkeys(vim.api.nvim_replace_termcodes("<Esc>", true, false, true), "n", true)
	end

	vim.fn.setreg("+", text)
	vim.notify("Copied: " .. text)
end, { desc = "Copy relative path (with lines in visual)" })
