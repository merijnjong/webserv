NAME		:= webserv
CXX			:= c++
CXXFLAGS	:= -g -Wextra -Wall -Werror -MMD

SRCS		:= main.cpp
OBJS		:= $(SRCS:.cpp=.o)
DEPS		:= $(OBJS:%.o=%.d)

all: $(NAME)

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -o $@ -c $< && printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) && printf "Linking: $(NAME)\n"

-include $(DEPS)

clean:
	@rm -rf $(OBJS) $(DEPS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re