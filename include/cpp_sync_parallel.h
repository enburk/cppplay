// The Asynchronous C++ Parallel Programming Model by Hartmut Kaiser [CppCon 2017]

future<T> traverse(tree_node const& t)
{
    if(t.has_children()) {
        std::array<future<T>, 8> results; // 8 for children

        for(int i= 0; i!= 8; ++i)
            results[i] = async(traverse, t.children[i]);

        return when_all(results, t.compute_result()).then(
            [](auto f, auto r) { return combine_results(f, r); }
        );
    }
    returnt.compute_result();
}

future<T> traverse(tree_node const& t)
{
    if(t.has_children()) {
        std::array<future<T>, 8> results; // 8 for children.

        for(int i= 0; i!= 8; ++i)
            results[i] = async(traverse, t.children[i]);

        co_return t.combine_results(co_await results, co_await t.compute_result());
    }
    co_return t.compute_result(); // perform calculations for leaf
}




hpx::future<void> step_future= make_ready_future();
for(std::size_tt= 0; t!= steps; ++t)
{
step_future= step_future.then(
[t](hpx::future<void> &&)
{
returnperform_one_time_step(t);
});
}
step_future.get(); // wait for everything to finish


hpx::future<void> partition::perform_one_time_step(intt)
{
// Update our boundaries from neighbors
hpx::future<void> top_boundary_future= channel_up_from.get(t)
.then([](hpx::future<std::vector<double>> && up_future)
{
std::vector<double> data = f.get(); // does not block
// process ghost-zone data using received data
// send new ghost zone data to neighbor
returnchannel_up_to.set(data);
});
// Apply stencil to partition
}


hpx::future<void> partition::perform_one_time_step(intt)
{
// Update our boundaries from neighbors
// Apply stencil to partition
hpx::future<void> interior_future=
hpx::parallel::for_loop(
par(task), min+1, max-1,
[](intidx)
{
// apply stencil to each point
});
// Join all asynchronous operations
}


hpx::future<void> partition::perform_one_time_step(intt)
{
// Update our boundaries from neighbors
// Apply stencil to partition
// Join all asynchronous operations
returnwhen_all(
top_boundary_future, bottom_boundary_future,
left_boundary_future, right_boundary_future,
interior_future);
}

// https://github.com/STEllAR-GROUP/hpx/